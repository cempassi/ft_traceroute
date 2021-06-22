/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:57:52 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/22 15:46:55 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <stdio.h>
#include <sysexits.h>


static int 	init_socket(t_traceroute *traceroute)
{
	int      icmp;

	icmp = 0;
	if ((traceroute->udp = socket(PF_INET, SOCK_DGRAM, 0)) < 0)
	{
		traceroute->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: Needs priviledged access\n", traceroute->name);
		return (-1);
	}
	if ((traceroute->icmp = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		traceroute->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: Needs priviledged access\n", traceroute->name);
		return (-1);
	}
	return (0);
}

static void init_traceroute(t_traceroute *traceroute, char **av)
{
    ft_bzero(traceroute, sizeof(t_traceroute));
    traceroute->name = av[0];
    traceroute->hops = DEFAULT_HOPS;
    traceroute->timeout = DEFAULT_TIMEOUT;
    traceroute->probes = DEFAULT_PROBES;
    traceroute->exit = 0;
    traceroute->payload_size = DEFAULT_PAYLOAD_LEN;
    traceroute->payload = DEFAULT_PAYLOAD;
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
