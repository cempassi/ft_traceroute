/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:05:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 00:52:12 by cempassi         ###   ########.fr       */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include "memory.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sysexits.h>

static int traceroute_loop(t_traceroute *traceroute, t_packet *template)
{
    uint8_t  ttl;
    uint8_t  seq;
    t_time   time;

    ttl = 1;
    while (ttl <= traceroute->hops && traceroute->finished == 0)
    {
        printf(" %d ", ttl);
        seq = 0;
        while (seq < traceroute->probes)
        {
            setup_iphdr(traceroute, template, ttl, seq);
            setup_udphdr(traceroute, template, traceroute->dest.sin_port);
            send_packets(traceroute, template, &time);
            traceroute->dest.sin_port += 1;
            select_packets(traceroute, &time);
            seq++;
        }
        printf("\n");
        ttl++;
    }
    return (0);
}

static int run_traceroute(t_traceroute *traceroute)
{
    t_packet *template;

    if ((template = ft_memalloc(traceroute->packet_size)) == NULL)
    {
        traceroute->exit = EX_OSERR;
        dprintf(2, "%s: payload allocation failed.\n", traceroute->name);
        return (-1);
    }
    printf("packet_size: %d| payload_size: %d\n", traceroute->packet_size, traceroute->payload_size);
    if (resolve_dst(traceroute) < 0) {
        return (-1);
    }
    generate_payload(traceroute, template);
    display_start(traceroute);
    traceroute_loop(traceroute, template);
    ft_memdel((void **)&template);
    // display_stats(ping);
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
