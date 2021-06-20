/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   display.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:41:07 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 20:48:51 by cempassi         ###   ########.fr       */
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

void display_ipheader(t_udppacket *packet)
{
     char src_ip[INET_ADDRSTRLEN];
     char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &packet->ipheader.src_addr, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &packet->ipheader.dst_addr, dst_ip, INET_ADDRSTRLEN);
    printf("-- ip header --\n"
              "version: %d\n"
              "ihl: %d\n"
              "tos: %d\n"
              "len: %d\n"
              "id: %d\n"
              "flags: %d\n"
              "flag offset: %d\n"
              "ttl: %d\n"
              "proto: %d\n"
              "checksum: %d\n"
              "src_addr: %s\n"
              "dst_addr: %s\n",
              packet->ipheader.version, packet->ipheader.ihl,
              packet->ipheader.tos, packet->ipheader.len, packet->ipheader.id,
              packet->ipheader.flags, packet->ipheader.flag_offset,
              packet->ipheader.ttl, packet->ipheader.proto,
              packet->ipheader.checksum, src_ip, dst_ip);
}

void display_udpheader(t_udpheader *udpheader)
{
    printf("-- udp header --\n"
              "src_port: %d\n"
              "dst_port: %d\n"
              "length: %d\n"
              "checksum: %d\n",
              udpheader->src_port, udpheader->dst_port, udpheader->lenght,
              udpheader->checksum);
}

void display_packet(t_udppacket *packet)
{
     char src_ip[INET_ADDRSTRLEN];
     char dst_ip[INET_ADDRSTRLEN];

    inet_ntop(AF_INET, &packet->ipheader.src_addr, src_ip, INET_ADDRSTRLEN);
    inet_ntop(AF_INET, &packet->ipheader.dst_addr, dst_ip, INET_ADDRSTRLEN);
    printf("\n-- ip header --\n"
              "version: %d\n"
              "ihl: %d\n"
              "tos: %d\n"
              "len: %d\n"
              "id: %d\n"
              "flags: %d\n"
              "flag offset: %d\n"
              "ttl: %d\n"
              "proto: %d\n"
              "checksum: %d\n"
              "src_addr: %s\n"
              "dst_addr: %s\n",
              packet->ipheader.version, packet->ipheader.ihl,
              packet->ipheader.tos, packet->ipheader.len, packet->ipheader.id,
              packet->ipheader.flags, packet->ipheader.flag_offset,
              packet->ipheader.ttl, packet->ipheader.proto,
              packet->ipheader.checksum, src_ip, dst_ip);

    printf("\n-- udp header --\n"
              "src_port: %d\n"
              "dst_port: %d\n"
              "length: %d\n"
              "checksum: %d\n",
              ntohs(packet->udpheader.src_port), ntohs(packet->udpheader.dst_port),
              packet->udpheader.lenght, packet->udpheader.checksum);
    printf("payload: %s\n", packet->payload);
}
