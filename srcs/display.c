/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:41:07 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/21 11:36:37 by cempassi         ###   ########.fr       */
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

void display_ipheader(t_ipheader *ipheader)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &ipheader->saddr, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &ipheader->daddr, dst_ip, INET_ADDRSTRLEN);
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
           ipheader->version, ipheader->ihl,
           ipheader->tos, ipheader->tot_len,
           ipheader->id, ipheader->flag_off,
           ipheader->ttl, ipheader->protocol,
           ipheader->check, src_ip, dst_ip);
}

void display_udppacket(t_udppacket *packet)
{
    printf("\n-- udp header --\n"
           "src_port: %d\n"
           "dst_port: %d\n"
           "length: %d\n"
           "checksum: %d\n",
           ntohs(packet->udpheader.uh_sport), ntohs(packet->udpheader.uh_dport),
           packet->udpheader.uh_ulen, packet->udpheader.uh_sum);
    printf("payload: %s\n", packet->payload);
}

void display_packet(t_packet *packet)
{
    char src_ip[INET_ADDRSTRLEN];
    char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &packet->ipheader.saddr, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &packet->ipheader.daddr, dst_ip, INET_ADDRSTRLEN);
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
           packet->ipheader.version, packet->ipheader.ihl,
           packet->ipheader.tos, packet->ipheader.tot_len,
           packet->ipheader.id, packet->ipheader.flag_off,
           packet->ipheader.ttl, packet->ipheader.protocol,
           packet->ipheader.check, src_ip, dst_ip);

    printf("\n-- udp header --\n"
           "src_port: %d\n"
           "dst_port: %d\n"
           "length: %d\n"
           "checksum: %d\n",
           ntohs(packet->udppacket.udpheader.uh_sport), ntohs(packet->udppacket.udpheader.uh_dport),
           packet->udppacket.udpheader.uh_ulen, packet->udppacket.udpheader.uh_sum);
    printf("payload: %s\n", packet->udppacket.payload);
}
