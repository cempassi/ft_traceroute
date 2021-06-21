/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 17:07:01 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/21 12:03:21 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <sysexits.h>

static void generate_payload(const t_traceroute *traceroute,
                             t_packet *packet, size_t size)
{
    size_t to_copy;
    size_t payload_len;

    payload_len = ft_strlen(traceroute->payload);
    to_copy = size / payload_len;
    while (to_copy)
    {
        ft_strlcat(packet->udppacket.payload, traceroute->payload, size);
        --to_copy;
    }
    if (size % payload_len)
        ft_strlcat(packet->udppacket.payload, traceroute->payload, size);
    return;
}

static size_t get_packet_size(t_traceroute *traceroute)
{
    size_t packet_size;

    if (traceroute->payload_size > MAX_PAYLOAD_SIZE)
        packet_size = MAX_PAYLOAD_SIZE + PACKET_HEADER_LEN;
    else
        packet_size = traceroute->payload_size + PACKET_HEADER_LEN;
    return (packet_size);
}

static void init_ipheader(t_packet *packet, size_t packet_size,
                          t_addrinfo *src, t_addrinfo *dst)
{
    packet->ipheader.version = 4;
    packet->ipheader.ihl = 5;
    packet->ipheader.tos = 16;
    packet->ipheader.tot_len = packet_size;
    packet->ipheader.id = 0;
    packet->ipheader.flag_off = 0;
    packet->ipheader.ttl = 1;
    packet->ipheader.protocol = IPPROTO_UDP;
    packet->ipheader.check = 0;
    packet->ipheader.saddr = ((struct sockaddr_in *)src->ai_addr)->sin_addr;
    packet->ipheader.daddr = ((struct sockaddr_in *)dst->ai_addr)->sin_addr;
}

void init_udpheader(t_packet *packet, t_addrinfo *src, t_addrinfo *dst)
{
    packet->udppacket.udpheader.uh_sport = ((struct sockaddr_in *)src->ai_addr)->sin_port;
    packet->udppacket.udpheader.uh_dport = ((struct sockaddr_in *)dst->ai_addr)->sin_port;
    packet->udppacket.udpheader.uh_sum = 0;
    packet->udppacket.udpheader.uh_ulen = UDP_HEADER_LEN + DEFAULT_PAYLOAD_LEN;
}

t_packet *generate_packet(t_traceroute *traceroute, t_addrinfo *src,
                             t_addrinfo *dst)
{
    t_packet *packet;
    size_t   packet_size;

    packet_size = get_packet_size(traceroute);
    if ((packet = ft_memalloc(packet_size)) == NULL)
    {
        traceroute->exit = EX_USAGE;
        dprintf(2, "%s: packet allocation failed\n", traceroute->name);
        return (NULL);
    }
    init_ipheader(packet, packet_size, src, dst);
    init_udpheader(packet, src, dst);
    generate_payload(traceroute, packet, packet_size - PACKET_HEADER_LEN);
    return (packet);
}
