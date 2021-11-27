/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:53:57 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 00:39:33 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <errno.h>
#include <stdio.h>

void setup_iphdr(t_traceroute *traceroute, t_packet *packet, uint8_t ttl,
                 uint16_t seq)
{
    t_ipheader *iphdr = &packet->ipheader;

    iphdr->version = 4;
    iphdr->ihl = IP_HEADER_LEN / 4;
    iphdr->tos = 0;
    iphdr->tot_len = htons(traceroute->packet_size);
    iphdr->id = htons((uint16_t)(DEFAULT_SRC_PORT + seq));
    iphdr->frag_off = htons(0);
    iphdr->protocol = IPPROTO_UDP;
    iphdr->check = 0;
    iphdr->saddr = INADDR_ANY;
    iphdr->daddr = traceroute->dest.sin_addr.s_addr;
    iphdr->ttl = ttl;
}

void setup_udphdr(t_traceroute *traceroute, t_packet *packet, uint16_t port)
{
    struct udphdr *udphdr = &packet->udpheader;

    udphdr->source = htons(DEFAULT_SRC_PORT);
    udphdr->dest = htons(port);
    udphdr->len = htons(traceroute->packet_size - IP_HEADER_LEN);
    udphdr->check = 0;
}
