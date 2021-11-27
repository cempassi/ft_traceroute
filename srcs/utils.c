/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/26 21:39:28 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 19:17:41 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include <errno.h>
#include <stdio.h>

uint16_t checksum(void *addr, int count)
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

void get_time(t_traceroute *traceroute, struct timeval *time)
{
    if (gettimeofday(time, NULL) < 0)
    {
        dprintf(STDERR_FILENO, "%s: gettimeofday(): %s\n", traceroute->name,
                strerror(errno));
    }
}

int generate_payload(t_traceroute *traceroute, t_packet *template)
{
    ft_memset(template->payload, 0x42, traceroute->payload_size -1);
    return(0);
}
