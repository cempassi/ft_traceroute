/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:05:45 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/24 22:30:20 by cempassi         ###   ########.fr       */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include "memory.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sysexits.h>

static int recv_packet(t_traceroute *traceroute, int socket)
{
    char            buffer[512];
    int             result;
    struct sockaddr address;
    socklen_t       address_len;

    ft_bzero(buffer, 512);
    result
        = recvfrom(socket, buffer, 512, MSG_DONTWAIT, &address, &address_len);
    if (result < 0)
    {
        dprintf(STDERR_FILENO, "%s: failed to recv packet\n", traceroute->name);
    }
    display_icmppacket((struct s_response *)buffer);
    return (0);
}

static int select_packets(t_traceroute *traceroute)
{
    struct timeval timeout;
    int            result;
    fd_set         set;

    result = 0;
    FD_ZERO(&set);
    FD_SET(traceroute->input, &set);
    timeout.tv_sec = traceroute->timeout;
    timeout.tv_usec = 0;
    result = select(traceroute->input + 1, &set, NULL, NULL, &timeout);
    if (result < 0)
    {
        dprintf(STDERR_FILENO, "%s: select failed\n", traceroute->name);
        return (-1);
    }
    else if (result == 0)
    {
        printf(" * ");
    }
    else if (recv_packet(traceroute, traceroute->input) < 0)
    {
        return (-1);
    }
    return (0);
}

#include <errno.h>
static int send_packets(t_traceroute *traceroute, t_packet *packet)
{
    int16_t send;

    send = sendto(traceroute->output, packet, traceroute->packet_size, 0,
                  (struct sockaddr *)&traceroute->dest,
                  sizeof(traceroute->dest));
    if (send < 0)
    {
        dprintf(STDERR_FILENO, "%s: failed to send packet, %s\n", traceroute->name, strerror(errno));
    }
    printf("Packet sent...\n");
    return (0);
}
uint16_t 	checksum(void *addr, int count)
{
	uint32_t  sum;
	uint16_t *ptr;

	sum = 0;
	ptr = addr;
	while (count > 1)
	{
		sum += *ptr++;
		count -= 2;
	}
	if (count > 0)
		sum += *(uint8_t *)ptr;
	while (sum >> 16)
		sum = (sum & 0xFFFF) + (sum >> 16);
	return ~sum;
}
static int traceroute_loop(t_traceroute *traceroute, t_packet *template)
{
    uint32_t ttl;
    uint8_t  seq;

    ttl = 1;
    seq = 0;
    while (ttl <= traceroute->hops)
    {
        while (seq < traceroute->probes * ttl)
        {
            setup_iphdr(traceroute, template, ttl, seq);
            setup_udphdr(traceroute, template, traceroute->dest_port);
            display_ipheader(&template->ipheader);
            display_udpheader(&template->udpheader, template->payload);
            traceroute->dest_port += 1;
            template->seq = seq;
            template->ttl = ttl;
            printf("--------- SENT PACKET ----------");
            send_packets(traceroute, template);
            printf("--------- RECV PACKET ----------");
            select_packets(traceroute);
        }
        ttl++;
    }
    return (0);
}

int init_packet(t_traceroute *traceroute, t_packet *template)
{
    resolve_dst(traceroute);
    template->payload = generate_payload(traceroute);
    return (0);
}

static int run_traceroute(t_traceroute *traceroute)
{
    t_packet template;

    if (init_packet(traceroute, &template) < 0)
    {
        return (-1);
    }
    display_start(traceroute);
    traceroute_loop(traceroute, &template);
    // display_stats(ping);
    return (0);
}

int main(int ac, char **av)
{
    t_traceroute traceroute;

    if (ac == 1)
    {
        display_help(av[0]);
        return (-1);
    }
    if (init_prgm(&traceroute, ac, av))
    {
        if (traceroute.exit == EX_USAGE)
            display_help(av[0]);
        return (traceroute.exit);
    }
    if (traceroute.options & OPT_H)
    {
        display_help(av[0]);
        return (0);
    }
    return (run_traceroute(&traceroute));
}
