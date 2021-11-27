/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 21:41:13 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 20:14:03 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include "memory.h"
#include <errno.h>
#include <stdio.h>

void delta_time(t_time *time)
{
    double delta;
    double sent;
    double recv;

    sent = (double)time->sent.tv_sec * 1000000.0 + (double)time->sent.tv_usec;
    recv = (double)time->recv.tv_sec * 1000000.0 + (double)time->recv.tv_usec;

    delta = (recv - sent) / 1000.0;
    if (delta < 0.1)
        printf("%.4f ms ", delta);
    else
        printf("%.3f ms ", delta);
}

static int process_packet(t_traceroute *traceroute, int socket, t_time *time)
{
    char      buffer[512];
    int       result;
    t_socket  address;
    socklen_t address_len;

    address_len = sizeof(address);
    ft_bzero(buffer, 512);
    ft_bzero(&address, sizeof(t_socket));
    result = recvfrom(socket, buffer, 512, MSG_DONTWAIT,
                      (struct sockaddr *)&address, &address_len);
    if (result < 0)
    {
        dprintf(STDERR_FILENO, "%s: failed to recv packet\n", traceroute->name);
        return (-1);
    }
    get_time(traceroute, &time->recv);
    resolve_node(traceroute, &address);
    delta_time(time);
    if (((t_response *)buffer)->header.type == ICMP_UNREACH)
    {
        traceroute->finished = 1;
    }
    return (0);
}

int recv_packets(t_traceroute *traceroute, t_time *time)
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
        ft_bzero(&traceroute->current, sizeof(t_socket));
        printf(" * ");
    }
    else if (process_packet(traceroute, traceroute->input, time) < 0)
    {
        return (-1);
    }
    return (0);
}

int send_packets(t_traceroute *traceroute, t_packet *packet, t_time *time)
{
    int16_t send;

    get_time(traceroute, &time->sent);
    send = sendto(traceroute->output, packet, traceroute->packet_size, 0,
                  (struct sockaddr *)&traceroute->dest,
                  sizeof(traceroute->dest));
    if (send < 0)
    {
        dprintf(STDERR_FILENO, "%s: failed to send packet, %s\n",
                traceroute->name, strerror(errno));
        return (-1);
    }
    traceroute->dest.sin_port = htons(ntohs(traceroute->dest.sin_port) + 1);
    return (0);
}
