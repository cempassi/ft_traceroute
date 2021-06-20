/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:41:07 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 15:29:41 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <arpa/inet.h>

void display_help(char *name)
{
    ft_dprintf(STDERR_FILENO, "usage: %s [-hoqv] [-c count] ", name);
    ft_dprintf(STDERR_FILENO, "[-i wait] [-m ttl] [-s packetsize] host\n");
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_C);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_I);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_M);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_S);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_H);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_O);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_Q);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_V);
}

void display_start(t_traceroute *traceroute, struct addrinfo *host)
{
    char                host_ip[INET_ADDRSTRLEN];
    int                 data;
    struct sockaddr_in *address;

    address = (struct sockaddr_in *)host->ai_addr;
    data = traceroute->payload_size;
    inet_ntop(AF_INET, &address->sin_addr, host_ip, INET_ADDRSTRLEN);
    ft_printf("traceroute to %s (%s), %d max hops, %d bytes packets\n",
              traceroute->host, host_ip, traceroute->hops,
              traceroute->payload_size + sizeof(t_udppacket));
}
