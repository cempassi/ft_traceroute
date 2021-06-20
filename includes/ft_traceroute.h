/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:09:35 by cempassi          #+#    #+#             */
/*   Updated: 2021/06/20 20:21:05 by cempassi         ###   ########.fr       */
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

typedef struct s_ipheader
{
    uint8_t  version : 4;
    uint8_t  ihl : 4;
    uint8_t  tos;
    uint16_t len;
    uint16_t id;
    uint16_t flags : 3;
    uint16_t flag_offset : 13;
    uint8_t  ttl;
    uint8_t  proto;
    uint16_t checksum;
    uint32_t src_addr;
    uint32_t dst_addr;
} t_ipheader;

typedef struct s_udpheader
{
    uint16_t src_port;
    uint16_t dst_port;
    uint16_t lenght;
    uint16_t checksum;
} t_udpheader;

typedef struct __attribute__((packed)) t_udppacket
{
    t_ipheader  ipheader;
    t_udpheader udpheader;
    char        payload[];
} t_udppacket;

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
    char     *host;
    char     *name;
    uint32_t options;
    uint32_t payload_size;
    char     *payload;
} t_traceroute;

int  init_prgm(t_traceroute *traceroute, int ac, char **av);

t_udppacket *generate_packet(t_traceroute *traceroute, t_addrinfo *dst, t_addrinfo *src);

/*
*****************************************************
********************* DISPLAY ***********************
*****************************************************
*/

void display_help(char *name);
void display_start(t_traceroute *traceroute, struct addrinfo *host);
void display_packet(t_udppacket *packet);

#endif
