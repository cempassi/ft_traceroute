/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:41:07 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 19:31:30 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_traceroute.h"
#include <arpa/inet.h>
#include <stdio.h>

void display_help(char *name)
{
    dprintf(STDERR_FILENO, "usage: %s [-hoqv] [-c count] ", name);
    dprintf(STDERR_FILENO, "[-i wait] [-m ttl] [-s packetsize] host\n");
}


void display_start(t_traceroute *traceroute)
{
    char      host_ip[INET_ADDRSTRLEN];
    t_socket *address;

    address = &traceroute->dest;
    inet_ntop(AF_INET, &address->sin_addr, host_ip, INET_ADDRSTRLEN);
    ft_printf("traceroute to %s (%s), %d max hops, %d bytes packets\n",
              traceroute->host, host_ip, traceroute->hops,
              traceroute->payload_size + sizeof(t_packet));
}
