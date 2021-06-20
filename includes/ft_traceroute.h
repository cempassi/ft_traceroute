/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:09:35 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 14:26:35 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H
#include "libft.h"
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#define OPTSTR "hv"
#define DEFAULT_HOPS 64

#define OPT_H 0x0001
#define OPT_V 0x0002

typedef struct addrinfo t_addrinfo;
typedef struct msghdr   t_msghdr;

typedef struct s_socket
{
    int fd;
    int ttl;
    int tos;
} t_socket;

typedef struct s_time
{
    struct timeval sent;
    struct timeval recv;
} t_time;

typedef struct s_iphdr
{
    uint8_t  version : 4;
    uint8_t  ihl : 4;
    uint8_t  tos;
    uint16_t len;
    uint16_t id;
    uint16_t flags : 3;
    uint16_t frag_offset : 13;
    uint8_t  ttl;
    uint8_t  proto;
    uint16_t csum;
    uint32_t saddr;
    uint32_t daddr;
} t_iphdr;

typedef struct s_traceroute
{
    t_socket udp;
    t_socket icmp;
    uint32_t hops;
    int16_t  exit;
    char     *host;
    char     *name;
    uint32_t options;
} t_traceroute;

void display_help(char *name);
int  init_prgm(t_traceroute *traceroute, int ac, char **av);

#endif
