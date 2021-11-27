/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 15:02:55 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 22:32:23 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <stdio.h>
#include <sysexits.h>

static int check_arg(t_traceroute *traceroute, uint8_t opt, int64_t number)
{
    if (opt == OPT_M && (number > MAX_TTL || number == 0))
    {
        dprintf(2, "%s: %s `%ld`\n", traceroute->name, OPT_M_E_STR, number);
        traceroute->exit = EX_USAGE;
    }
    if (opt == OPT_Q && (number > MAX_PROBES || number == 0))
    {
        dprintf(2, "%s: %s `%ld`\n", traceroute->name, OPT_Q_E_STR, number);
        traceroute->exit = EX_USAGE;
    }
    if (opt == OPT_W && (number > MAX_WAIT || number == 0))
    {
        dprintf(2, "%s: %s `%ld`\n", traceroute->name, OPT_W_E_STR, number);
        traceroute->exit = EX_USAGE;
    }
    return (traceroute->exit == EX_USAGE ? -1 : number);
}

static int get_argument(t_traceroute *traceroute, char *optarg, uint8_t opt,
                        char o)
{
    uint32_t convert;

    if (!ft_strcheck(optarg, ft_isdigit))
    {
        traceroute->exit = EX_USAGE;
        ft_dprintf(2, "%s: Argument is not a number -- %c\n", traceroute->name,
                   o);
        return (-1);
    }
    convert = ft_atoi(optarg);
    traceroute->options |= opt;
    return (check_arg(traceroute, opt, convert));
}

static int parse_opt(t_traceroute *traceroute, t_opt *option, int ac, char **av)
{
    char  o;
    char *optarg;

    optarg = NULL;
    while ((o = ft_getopt(ac, av, option, &optarg)) > 0
           && traceroute->exit == 0)
    {
        if (o == 'h')
            traceroute->options |= OPT_H;
        else if (o == 'm')
            traceroute->hops = get_argument(traceroute, optarg, OPT_M, o);
        else if (o == 'q')
            traceroute->probes = get_argument(traceroute, optarg, OPT_Q, o);
        else if (o == 'w')
            traceroute->timeout = get_argument(traceroute, optarg, OPT_W, o);
    }
    return (traceroute->exit);
}

int init_option(t_traceroute *traceroute, int ac, char **av)
{
    t_opt option;
    int   error;
    char *optstr;
    char *optlong[5];

    option.optstr = OPTSTR;
    option.first_arg = 1;
    optstr = NULL;
    optlong[0] = "max-hops=m:";
    optlong[1] = "queries=q:";
    optlong[2] = "wait=w:";
    optlong[3] = "help=h";
    optlong[4] = NULL;
    option.optlong = optlong;
    if ((error = ft_getopt(ac, av, &option, &optstr)))
    {
        traceroute->exit = EX_USAGE;
        if (ft_strchr(OPTSTR, error))
            ft_dprintf(2, "%s: Requires an argument -- -%c\n", av[0], error);
        else
            ft_dprintf(2, "%s: illegal option\n", av[0]);
        return (-1);
    }
    if (parse_opt(traceroute, &option, ac, av))
        return (-1);
    if (traceroute->options & OPT_H)
        return (0);
    if (option.first_arg != (size_t)ac - 1)
    {
        ft_dprintf(2, "%s: Host is missing \n", av[0]);
        traceroute->exit = EX_USAGE;
        return (-1);
    }
    return (0);
}
