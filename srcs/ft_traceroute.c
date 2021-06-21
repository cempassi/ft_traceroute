/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:05:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/21 16:12:11 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <arpa/inet.h>
#include <stdint.h>
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

static 	int set_ttl(int socket, int ttl)
{
	if (setsockopt(socket, IPPROTO_IP, IP_TTL, &ttl, sizeof(int)))
	{
		return (-1);
	}
	return (0);
}

static int send_packet(t_traceroute *traceroute, t_addrinfo *dst)
{
    int16_t send;
    char    *payload;

    payload = generate_payload(traceroute);
    set_ttl(traceroute->udp.fd, 1);
    send = sendto(traceroute->udp.fd, payload, traceroute->payload_size, 0,
                  dst->ai_addr, dst->ai_addrlen);
    if (send < 0)
    {
        dprintf(STDERR_FILENO, "%s: failed to send packet\n",
                traceroute->name);
    }
    printf("Packet sent...\n");
    return (0);
}

static int traceroute_loop(t_traceroute *traceroute, t_addrinfo *dst)
{
    send_packet(traceroute, dst);
    // ft_memdel((void **)&packet);
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
