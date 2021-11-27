/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:41:07 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/26 21:36:45 by cempassi         ###   ########.fr       */
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

void    print_bytes(int bytes, void *msg)
{
    for (int i = 0; i < bytes; i++)
    {
        if (!(i & 15))
            printf("\n[DEBUG] %04X:  ", i);
        printf("%02X ", ((unsigned char *)msg)[i]);
    }
    printf("\n");
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

void display_udpheader(t_udpheader *header, char *payload)
{
    printf("\n-- udp header --\n"
           "src_port: %d\n"
           "dst_port: %d\n"
           "length: %d\n"
           "checksum: %d\n",
           ntohs(header->uh_sport), ntohs(header->uh_dport),
           ntohs(header->uh_ulen), header->uh_sum);
    printf("payload: %s\n", payload);
}

void display_ipheader(t_ipheader *header)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &header->saddr, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &header->daddr, dst_ip, INET_ADDRSTRLEN);
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
           header->version, header->ihl, header->tos, ntohs(header->tot_len),
           header->id, header->frag_off, header->ttl, header->protocol,
           header->check, src_ip, dst_ip);
}

void display_response(t_response *packet, int recieved)
{
    printf("Packet recieved..., data: %d\n", recieved);
    printf("\n-- icmp header --\n"
           "type: %d\n"
           "code: %d\n"
           "checksum: %d\n",
           packet->header.type, packet->header.code,
           packet->header.checksum);
    display_ipheader(&packet->inner);
    display_udpheader(&packet->udp, packet->payload);
}
