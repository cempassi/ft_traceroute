/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   packet.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 17:07:01 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/26 21:33:20 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_traceroute.h"
#include "memory.h"
#include <arpa/inet.h>
#include <stdint.h>
#include <stdio.h>
#include <sysexits.h>

int generate_payload(t_traceroute *traceroute, t_packet *template)
{
    ft_memset(template->payload, 0x42, traceroute->payload_size -1);
    return(0);
}
