/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:41:07 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/24 22:07:15 by cempassi         ###   ########.fr       */
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
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_C);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_I);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_M);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_S);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_H);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_O);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_Q);
    // ft_dprintf(STDERR_FILENO, "\t%s\n", DESC_V);
}

void display_start(t_traceroute *traceroute)
{
    char                host_ip[INET_ADDRSTRLEN];
    t_socket            *address;

    address = &traceroute->dest;
    inet_ntop(AF_INET, &address->sin_addr, host_ip, INET_ADDRSTRLEN);
    ft_printf("traceroute to %s (%s), %d max hops, %d bytes packets\n",
              traceroute->host, host_ip, traceroute->hops,
              traceroute->payload_size + sizeof(t_packet));
}

void display_udpheader(t_udpheader *header, char *payload)
{
    printf("\n-- udp header --\n"
           "src_port: %d\n"
           "dst_port: %d\n"
           "length: %d\n"
           "checksum: %d\n",
           ntohs(header->uh_sport), ntohs(header->uh_dport),
           header->uh_ulen, header->uh_sum);
    printf("payload: %s\n", payload);
}

void display_ipheader(t_ipheader *header)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &header->ip_src, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &header->ip_dst, dst_ip, INET_ADDRSTRLEN);
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
           header->ip_v, header->ip_hl,
           header->ip_tos, ntohs(header->ip_len),
           header->ip_id, header->ip_off,
           header->ip_ttl, header->ip_p,
           header->ip_sum, src_ip, dst_ip);
}

void display_icmppacket(struct s_response *packet)
{
    printf("\n-- icmp header --\n"
            "type: %d\n"
            "code: %d\n"
            "checksum: %d\n",
            packet->header.type,
            packet->header.code,
            packet->header.checksum);
    display_ipheader(&packet->packet.ipheader);
    display_udpheader(&packet->packet.udpheader, packet->packet.payload);
}
