/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:05:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 18:06:58 by cempassi         ###   ########.fr       */
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

static int traceroute_loop(t_traceroute *traceroute, t_addrinfo *host)
{
    t_udppacket *packet;

    (void)host;
    if ((packet = generate_packet(traceroute)) == NULL)
    {
        return (-1);
    }
    display_packet(packet);
    //ft_memdel((void **)&packet);
    return (0);
}

static int run_traceroute(t_traceroute *traceroute)
{
    t_addrinfo *host;

    if ((host = resolve_host(traceroute)) == NULL)
    {
        return (-1);
    }
    display_start(traceroute, host);
    traceroute_loop(traceroute, host);
    // display_stats(ping);
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
