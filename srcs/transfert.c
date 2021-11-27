/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   transfert.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 21:41:13 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 18:32:13 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include "memory.h"
#include <stdio.h>
#include <errno.h>


static int recv_packet(t_traceroute *traceroute, int socket, t_time *time)
{
    char            buffer[512];
    int             result;
    t_socket        address;
    socklen_t       address_len;
    t_response      *response;

    (void)time;
    address_len = sizeof(address);
    ft_bzero(buffer, 512);
    ft_bzero(&address, sizeof(t_socket));
    result
        = recvfrom(socket, buffer, 512, MSG_DONTWAIT, (struct sockaddr*)&address, &address_len);
    if (result < 0)
    {
        dprintf(STDERR_FILENO, "%s: failed to recv packet\n", traceroute->name);
    }
    response = (t_response *)buffer;
    printf("type: %d ", response->header.type);
    resolve_response(traceroute, (t_response *)buffer, &address);
    if (((t_response *)buffer)->header.type == ICMP_UNREACH) {
        printf("I'm");
        traceroute->finished = 1;
    }
    return (0);
}

int select_packets(t_traceroute *traceroute, t_time *time)
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
    else if (recv_packet(traceroute, traceroute->input, time) < 0)
    {
        return (-1);
    }
    get_time(traceroute, &time->recv);
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
        dprintf(STDERR_FILENO, "%s: failed to send packet, %s\n", traceroute->name, strerror(errno));
    }
    return (0);
}

