/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:05:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/22 19:52:35 by cempassi         ###   ########.fr       */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include "memory.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sysexits.h>

static struct addrinfo *resolve_dst(t_traceroute *traceroute)
{
    struct addrinfo *host;
    struct addrinfo  hints;
    int              error;

    host = NULL;
    ft_bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_ADDRCONFIG | AI_V4MAPPED;
    if ((error = getaddrinfo(traceroute->host, "33434", &hints, &host)))
    {
        traceroute->exit = EX_NOHOST;
        if (error == EAI_NONAME)
            ft_dprintf(STDERR_FILENO, "%s: cannot resolve %s: host unknown\n",
                       traceroute->name, traceroute->host);
        else
            ft_dprintf(STDERR_FILENO, "%s: failed to reach host\n",
                       traceroute->name);
        return (NULL);
    }
    return (host);
}

static struct addrinfo *generate_dst(t_traceroute *traceroute)
{
    struct addrinfo *host;
    struct addrinfo  hints;
    int              error;

    host = NULL;
    ft_bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_PASSIVE;
    if ((error = getaddrinfo(NULL, "33434", &hints, &host)))
    {
        traceroute->exit = EX_NOHOST;
        if (error == EAI_NONAME)
            ft_dprintf(STDERR_FILENO, "%s: cannot resolve %s: host unknown\n",
                       traceroute->name, traceroute->host);
        else
            ft_dprintf(STDERR_FILENO, "%s: failed to reach host\n",
                       traceroute->name);
        return (NULL);
    }
    return (host);
}
static int set_ttl(int socket, int ttl)
{
    if (setsockopt(socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)))
    {
        return (-1);
    }
    return (0);
}

static int send_packets(t_traceroute *traceroute, char *payload,
                        t_addrinfo *dst)
{
    int16_t send;
    uint8_t sent;

    sent = 0;
    while (sent < traceroute->probes)
    {
        send = sendto(traceroute->udp, payload, traceroute->payload_size, 0,
                      dst->ai_addr, dst->ai_addrlen);
        if (send < 0)
        {
            dprintf(STDERR_FILENO, "%s: failed to send packet\n",
                    traceroute->name);
        }
        printf("Packet sent...\n");
        sent++;
    }
    return (0);
}

static int recv_packet(t_traceroute *traceroute)
{
    char            buffer[512];
    int             result;
    struct sockaddr address;
    socklen_t       address_len;

    ft_bzero(buffer, 512);
    result = recvfrom(traceroute->icmp, buffer, 512, MSG_DONTWAIT, &address,
                      &address_len);
    if (result < 0)
    {
        dprintf(STDERR_FILENO, "%s: failed to recv packet\n", traceroute->name);
    }
    display_icmppacket((t_icmppacket *)buffer);
    return (0);
}

static int select_packets(t_traceroute *traceroute)
{
    struct timeval  timeout;
    struct addrinfo *binding;
    uint8_t         probe;
    int             result;
    fd_set          set;
    int             fake_socket;

    probe = 0;
    result = 0;
    binding = generate_dst(traceroute);
    FD_ZERO(&set);
	if ((fake_socket = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		traceroute->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: Needs priviledged access\n", traceroute->name);
		return (-1);
	}
    FD_SET(traceroute->icmp, &set);
    while (probe++ < traceroute->probes)
    {
        timeout.tv_sec = traceroute->timeout;
        timeout.tv_usec = 0;
        if (bind(traceroute->icmp, binding->ai_addr, binding->ai_addrlen))
        {
            dprintf(STDERR_FILENO, "%s: binding failed\n", traceroute->name);
            return (-1);
        }
        result = select(traceroute->icmp + 1, &set, NULL, NULL, &timeout);
        if (result < 0)
        {
            dprintf(STDERR_FILENO, "%s: select failed\n", traceroute->name);
            return (-1);
        }
        else if (result == 0)
        {
            printf(" * ");
            continue;
        }
        else if (recv_packet(traceroute) < 0)
        {
            return (-1);
        }
    }
    return (0);
}

static int traceroute_loop(t_traceroute *traceroute, t_addrinfo *dst)
{
    char *   payload;
    uint32_t step;

    payload = generate_payload(traceroute);
    step = 1;
    while (step <= traceroute->hops)
    {
        set_ttl(traceroute->udp, step);
        send_packets(traceroute, payload, dst);
        select_packets(traceroute);
        step++;
    }
    return (0);
}

static int run_traceroute(t_traceroute *traceroute)
{
    t_addrinfo *dst;

    if ((dst = resolve_dst(traceroute)) == NULL)
    {
        return (-1);
    }
    display_start(traceroute, dst);
    traceroute_loop(traceroute, dst);
    // display_stats(ping);
    freeaddrinfo(dst);
    return (0);
}

int main(int ac, char **av)
{
    t_traceroute traceroute;

    if (ac == 1)
    {
        display_help(av[0]);
        return (-1);
    }
    if (init_prgm(&traceroute, ac, av))
    {
        if (traceroute.exit == EX_USAGE)
            display_help(av[0]);
        return (traceroute.exit);
    }
    if (traceroute.options & OPT_H)
    {
        display_help(av[0]);
        return (0);
    }
    return (run_traceroute(&traceroute));
}
