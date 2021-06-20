/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:05:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 20:36:55 by cempassi         ###   ########.fr       */
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
    hints.ai_socktype = SOCK_RAW;
    hints.ai_flags = AI_ADDRCONFIG;
    if ((error = getaddrinfo(traceroute->host, DEFAULT_DST_PORT, &hints, &host)))
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

static t_addrinfo *bind_src(t_traceroute *traceroute)
{
    struct addrinfo *host;
    struct addrinfo  hints;
    int              error;

    host = NULL;
    ft_bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_flags = AI_PASSIVE;
    if ((error = getaddrinfo(NULL, DEFAULT_SRC_PORT, &hints, &host)))
    {
        traceroute->exit = EX_NOHOST;
        if (error == EAI_NONAME)
            dprintf(STDERR_FILENO, "%s: cannot resolve %s: host unknown\n",
                    traceroute->name, traceroute->host);
        else
            dprintf(STDERR_FILENO, "%s: failed to reach host\n",
                    traceroute->name);
        return (NULL);
    }
    return (host);
}

static int send_packet(t_traceroute *traceroute, t_udppacket *packet,
                       t_addrinfo *dst)
{
    int16_t send;

    send = sendto(traceroute->udp.fd, packet, packet->ipheader.len, 0,
                  dst->ai_addr, dst->ai_addrlen);
    if (send < 0)
    {
        dprintf(STDERR_FILENO, "%s: failed to send packet\n",
                traceroute->name);
    }
    printf("Packet sent...\n");
    return (0);
}

static int traceroute_loop(t_traceroute *traceroute, t_addrinfo *src,
                           t_addrinfo *dst)
{
    t_udppacket *packet;

    if ((packet = generate_packet(traceroute, src, dst)) == NULL)
    {
        return (-1);
    }
    display_packet(packet);
    send_packet(traceroute, packet, dst);
    // ft_memdel((void **)&packet);
    return (0);
}

static int run_traceroute(t_traceroute *traceroute)
{
    t_addrinfo *dst;
    t_addrinfo *src;

    if ((dst = resolve_dst(traceroute)) == NULL)
    {
        return (-1);
    }
    if ((src = bind_src(traceroute)) == NULL)
    {
        return (-1);
    }
    display_start(traceroute, dst);
    traceroute_loop(traceroute, src, dst);
    // display_stats(ping);
    freeaddrinfo(src);
    freeaddrinfo(dst);
    return (0);
}

int main(int ac, char **av)
{
    t_traceroute traceroute;

    if (ac == 1)
    {
        display_help(av[0]);
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
