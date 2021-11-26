/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 17:07:01 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/23 16:08:57 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include "memory.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <sysexits.h>

char *generate_payload(t_traceroute *traceroute)
{
    char  *payload;
    size_t to_copy;
    size_t payload_len;

    payload_len = ft_strlen(traceroute->payload);
    to_copy = traceroute->payload_size / payload_len;
    if ((payload = ft_memalloc(traceroute->payload_size)) == NULL)
    {
        traceroute->exit = EX_OSERR;
        dprintf(2, "%s: payload allocation failed.\n", traceroute->name);
        return (NULL);
    }
    while (to_copy)
    {
        ft_strlcat(payload, traceroute->payload,traceroute->payload_size);
        --to_copy;
    }
    if (traceroute->payload_size % payload_len)
        ft_strlcat(payload, traceroute->payload, traceroute->payload_size);
    return(payload);
}
