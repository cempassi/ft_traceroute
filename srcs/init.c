/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:57:52 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 19:18:52 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <errno.h>
#include <stdio.h>
#include <sysexits.h>

static int init_socket(t_traceroute *traceroute)
{
    int on = 1;

    if ((traceroute->output = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
    {
        traceroute->exit = EX_OSERR;
        dprintf(STDERR_FILENO, "%s: Needs priviledged access\n",
                traceroute->name);
        return (-1);
    }

    if ((setsockopt(traceroute->output, IPPROTO_IP, IP_HDRINCL, &on,
                    sizeof(on)))
        < 0)
    {
        dprintf(STDERR_FILENO, "ft_traceroute: setsockopt failed: %s\n",
                strerror(errno));
        return (-1);
    }

    if ((traceroute->input = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
    {
        traceroute->exit = EX_OSERR;
        dprintf(STDERR_FILENO, "%s: Needs priviledged access\n",
                traceroute->name);
        return (-1);
    }
    return (0);
}

static void init_traceroute(t_traceroute *traceroute, char **av)
{
    uint32_t packet_size = IP_HEADER_LEN + UDP_HEADER_LEN + DEFAULT_PAYLOAD_LEN;

    ft_bzero(traceroute, sizeof(t_traceroute));
    traceroute->name = av[0];
    traceroute->hops = DEFAULT_HOPS;
    traceroute->timeout = DEFAULT_TIMEOUT;
    traceroute->probes = DEFAULT_PROBES;
    traceroute->exit = 0;
    traceroute->payload_size = DEFAULT_PAYLOAD_LEN;
    traceroute->payload = DEFAULT_PAYLOAD;
    traceroute->packet_size = packet_size;
}

int init_prgm(t_traceroute *traceroute, int ac, char **av)
{
    setbuf(stdout, NULL);
    init_traceroute(traceroute, av);
    if (init_option(traceroute, ac, av) < 0)
    {
        return (-1);
    }
    traceroute->host = av[ac - 1];
    return (init_socket(traceroute));
}
