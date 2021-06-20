/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:57:52 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 14:52:44 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <stdio.h>
#include <sysexits.h>

// static int 	check_arg(t_traceroute *ping, int64_t number)
// {
// 	int e;
//
// 	e = 0;
// 	return (ping->exit == EX_USAGE ? -1 : number);
// }
//
// static int 	get_argument(t_traceroute *ping, char *optarg, uint8_t opt, char o)
// {
// 	uint32_t convert;
//
// 	if (!ft_strcheck(optarg, ft_isdigit))
// 	{
// 		ping->exit = EX_USAGE;
// 		ft_dprintf(2, "%s: Argument is not a number -- %c\n", ping->name, o);
// 		return (-1);
// 	}
// 	convert = ft_atoi(optarg);
// 	ping->options |= opt;
// 	return (check_arg(ping, convert));
// }

static 	int set_sockets_opt(t_traceroute *traceroute)
{
	t_socket 	   	*udp;
	t_socket 	   	*icmp;
	int 			broadcast;
    int             one;

    one = 1;
	icmp = &traceroute->icmp;
	udp = &traceroute->udp;
	broadcast = 1;
	if (setsockopt(icmp->fd, SOL_SOCKET, SO_BROADCAST, &broadcast, sizeof(int)))
	{
		traceroute->exit = EX_OSERR;
		ft_dprintf(2, "%s: SO_BROADCAST configuration failed\n", traceroute->name);
		return (-1);
	}
    if(setsockopt(udp->fd, IPPROTO_IP, IP_HDRINCL, &one, sizeof(one)) < 0)
	{
		traceroute->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: IPHDR configuration failed\n", traceroute->name);
		return (-1);
	}
	return (0);
}

static int 	init_socket(t_traceroute *traceroute)
{
	t_socket *udp;
	t_socket *icmp;

	udp = &traceroute->udp;
	icmp = &traceroute->icmp;
	if ((udp->fd = socket(PF_INET, SOCK_RAW, IPPROTO_UDP)) < 0)
	{
		traceroute->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: Needs priviledged access\n", traceroute->name);
		return (-1);
	}
	if ((icmp->fd = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
	{
		traceroute->exit = EX_OSERR;
		ft_dprintf(STDERR_FILENO, "%s: Needs priviledged access\n", traceroute->name);
		return (-1);
	}
	return (set_sockets_opt(traceroute) < 0 ? -1 : 0);
}

static int 	parse_opt(t_traceroute *ping, t_opt *option, int ac, char **av)
{
	char  o;
	char *optarg;

	while ((o = ft_getopt(ac, av, option, &optarg)) > 0 && ping->exit == 0)
	{
		if (o == 'v')
			ping->options |= OPT_V;
		else if (o == 'h')
			ping->options |= OPT_H;
	}
	return (ping->exit);
}

static void init_traceroute(t_traceroute *traceroute, t_opt *option, char **av)
{
    ft_bzero(traceroute, sizeof(t_traceroute));
    traceroute->hops = DEFAULT_HOPS;
    traceroute->exit = 0;
    traceroute->name = av[0];
    option->optstr = OPTSTR;
    option->first_arg = 1;
    option->optlong = NULL;
}


int init_prgm(t_traceroute *traceroute, int ac, char **av)
{
    t_opt option;
    int   error;

    setbuf(stdout, NULL);
    init_traceroute(traceroute, &option, av);
    if ((error = ft_getopt(ac, av, &option, NULL)))
    {
        traceroute->exit = EX_USAGE;
        if (ft_strchr(OPTSTR, error))
            ft_dprintf(2, "%s: Requires an argument -- -%c\n", av[0], error);
        else
            ft_dprintf(2, "%s: illegal option -- -%c\n", av[0], error);
        return (-1);
    }
	if (option.first_arg != (size_t)ac - 1)
	{
		traceroute->exit = EX_USAGE;
		return (-1);
	}
	traceroute->host = av[ac - 1];
	if (parse_opt(traceroute, &option, ac, av))
		return (-1);
    return (init_socket(traceroute));
}
