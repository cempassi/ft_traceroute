/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   setup.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/24 20:53:57 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/24 22:13:48 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"

void setup_iphdr(t_traceroute *traceroute, t_packet *packet, uint8_t ttl,
                 uint16_t seq)
{
    t_ipheader *iphdr = &packet->ipheader;

    iphdr->ip_v = 4;
    iphdr->ip_hl = IP_HEADER_LEN / 4;
    iphdr->ip_tos = 0;
    iphdr->ip_len = htons(traceroute->packet_size);
    iphdr->ip_id = htons((uint16_t)(DEFAULT_SRC_PORT + seq));
    iphdr->ip_off = htons(0);
    iphdr->ip_ttl = ttl;
    iphdr->ip_p = AF_INET;
    iphdr->ip_sum = 0;
    iphdr->ip_src.s_addr = INADDR_ANY;
    iphdr->ip_dst.s_addr = traceroute->dest.sin_addr.s_addr;
    (void)iphdr;
}

void setup_udphdr(t_traceroute *traceroute, t_packet *packet, uint16_t port)
{
    struct udphdr *udphdr = &packet->udpheader;

    udphdr->source = htons(DEFAULT_SRC_PORT);
    udphdr->dest = htons(port);
    udphdr->len = htons(traceroute->packet_size - IP_HEADER_LEN);
    udphdr->check = 0;
}
