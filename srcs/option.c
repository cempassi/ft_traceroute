/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   option.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/22 15:02:55 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/22 16:27:51 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <stdio.h>
#include <sysexits.h>

static int check_arg(t_traceroute *traceroute, int64_t number)
{
    int e;

    e = 0;
    if (traceroute->hops > MAX_TTL)
    {
        e = ft_dprintf(2, "%s: %s `%d`\n", traceroute->name, OPT_M_E_STR,
                       number);
    }
    return (traceroute->exit == EX_USAGE ? -1 : number);
}

static int get_argument(t_traceroute *ping, char *optarg, uint8_t opt, char o)
{
    uint32_t convert;

    if (!ft_strcheck(optarg, ft_isdigit))
    {
        ping->exit = EX_USAGE;
        ft_dprintf(2, "%s: Argument is not a number -- %c\n", ping->name, o);
        return (-1);
    }
    convert = ft_atoi(optarg);
    ping->options |= opt;
    return (check_arg(ping, convert));
}

static int parse_opt(t_traceroute *traceroute, t_opt *option, int ac, char **av)
{
    char  o;
    char *optarg;

    optarg = NULL;
    while ((o = ft_getopt(ac, av, option, &optarg)) > 0
           && traceroute->exit == 0)
    {
        if (o == 'v')
            traceroute->options |= OPT_V;
        else if (o == 'h')
            traceroute->options |= OPT_H;
        else if (o == 'm')
            traceroute->hops = get_argument(traceroute, optarg, OPT_M, o);
    }
    return (traceroute->exit);
}

int init_option(t_traceroute *traceroute, int ac, char **av)
{
    t_opt option;
    int   error;
    char *optstr;
    char *optlong[2];

    option.optstr = OPTSTR;
    option.first_arg = 1;
    optstr = NULL;
    optlong[0] = "max-hops=m:";
    optlong[1] = NULL;
    option.optlong = optlong;
    if ((error = ft_getopt(ac, av, &option, &optstr)))
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
    if (parse_opt(traceroute, &option, ac, av))
        return (-1);
    return (0);
}
