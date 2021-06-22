/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:41:07 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/22 12:26:40 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"
#include "ft_traceroute.h"
#include <arpa/inet.h>
#include <stdio.h>

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
    struct sockaddr_in *address;

    address = (struct sockaddr_in *)host->ai_addr;
    inet_ntop(AF_INET, &address->sin_addr, host_ip, INET_ADDRSTRLEN);
    ft_printf("traceroute to %s (%s), %d max hops, %d bytes packets\n",
              traceroute->host, host_ip, traceroute->hops,
              traceroute->payload_size + sizeof(t_udppacket));
}

void display_udppacket(t_udppacket *packet)
{
    printf("\n-- udp header --\n"
           "src_port: %d\n"
           "dst_port: %d\n"
           "length: %d\n"
           "checksum: %d\n",
           ntohs(packet->header.uh_sport), ntohs(packet->header.uh_dport),
           packet->header.uh_ulen, packet->header.uh_sum);
    printf("payload: %s\n", packet->payload);
}

void display_ippacket(t_ippacket *packet)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &packet->header.saddr, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &packet->header.daddr, dst_ip, INET_ADDRSTRLEN);
    printf("\n-- ip header --\n"
           "version: %d\n"
           "ihl: %d\n"
           "tos: %d\n"
           "len: %d\n"
           "id: %d\n"
           "flag offset: %d\n"
           "ttl: %d\n"
           "proto: %d\n"
           "checksum: %d\n"
           "src_addr: %s\n"
           "dst_addr: %s\n",
           packet->header.version, packet->header.ihl,
           packet->header.tos, packet->header.tot_len,
           packet->header.id, packet->header.frag_off,
           packet->header.ttl, packet->header.protocol,
           packet->header.check, src_ip, dst_ip);
}

void display_icmppacket(t_icmppacket *packet)
{
    printf("\n-- icmp header --\n"
            "type: %d\n"
            "code: %d\n"
            "checksum: %d\n"
            "data: %d\n",
            packet->header.type,
            packet->header.code,
            packet->header.checksum,
            packet->header.data);
    display_ippacket(&packet->ippacket);
    display_udppacket(&packet->ippacket.udppacket);
}
