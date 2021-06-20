/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:05:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 17:08:16 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <sysexits.h>

static struct addrinfo *resolve_host(t_traceroute *traceroute)
{
    struct addrinfo *host;
    struct addrinfo  hints;
    int              error;

    host = NULL;
    ft_bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_RAW;
    hints.ai_flags = AI_ADDRCONFIG;
    if ((error = getaddrinfo(traceroute->host, NULL, &hints, &host)))
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

static int run_traceroute(t_traceroute *traceroute)
{
    struct addrinfo *host;
    t_udppacket *    packet;

    if ((packet = generate_packet(traceroute)) == NULL)
    {
        return (-1);
    }
    if ((host = resolve_host(traceroute)) == NULL)
    {
        return (-1);
    }
    display_start(traceroute, host);
    // traceroute_loop(ping, host, packet);
    // display_stats(ping);
    ft_memdel((void **)&packet);
    freeaddrinfo(host);
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
