/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:09:35 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/21 11:34:15 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H
#include "libft.h"
#include <netdb.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdint.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#define OPTSTR "hv"

#define DEFAULT_HOPS 64
#define DEFAULT_PAYLOAD "42"
#define DEFAULT_PAYLOAD_LEN 50
#define DEFAULT_SRC_PORT "3490"
#define DEFAULT_DST_PORT "33434"

#define OPT_H 0x0001
#define OPT_V 0x0002

#define MAX_PAYLOAD_SIZE 64
#define UDP_HEADER_LEN 8
#define IP_HEADER_LEN 20
#define PACKET_HEADER_LEN UDP_HEADER_LEN + IP_HEADER_LEN
#define TIME_DATA sizeof(t_time)

typedef struct addrinfo t_addrinfo;
typedef struct msghdr   t_msghdr;
typedef struct udphdr   t_udpheader;

typedef struct s_ipheader
{
#if __BYTE_ORDER == __LITTLE_ENDIAN
    unsigned int ihl : 4;
    unsigned int version : 4;
#elif __BYTE_ORDER == __BIG_ENDIAN
    unsigned int version : 4;
    unsigned int ihl : 4;
#else
#error "Please fix <bits/endian.h>"
#endif
    u_int8_t       tos;
    u_int16_t      tot_len;
    u_int16_t      id;
    u_int16_t      flag_off;
    u_int8_t       ttl;
    u_int8_t       protocol;
    u_int16_t      check;
    struct in_addr saddr;
    struct in_addr daddr;
    /*The options start here. */
} t_ipheader;

typedef struct __attribute__((packed)) t_udppacket
{
    t_udpheader udpheader;
    char        payload[];
} t_udppacket;

typedef struct __attribute__((packed)) s_packet
{
    t_ipheader  ipheader;
    t_udppacket udppacket;
} t_packet;

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

typedef struct s_traceroute
{
    t_socket udp;
    t_socket icmp;
    uint32_t hops;
    int16_t  exit;
    char *   host;
    char *   name;
    uint32_t options;
    uint32_t payload_size;
    char *   payload;
} t_traceroute;

int init_prgm(t_traceroute *traceroute, int ac, char **av);

t_packet *generate_packet(t_traceroute *traceroute, t_addrinfo *dst,
                          t_addrinfo *src);

/*
*****************************************************
********************* DISPLAY ***********************
*****************************************************
*/

void display_help(char *name);
void display_start(t_traceroute *traceroute, struct addrinfo *host);
void display_packet(t_packet *packet);

#endif
