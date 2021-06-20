/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:05:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 12:27:36 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <sysexits.h>

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
}
