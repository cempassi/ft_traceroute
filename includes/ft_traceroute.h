/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_traceroute.h                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cempassi <cempassi@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/06/20 11:09:35 by cempassi          #+#    #+#             */
/*   Updated: 2021/11/27 17:35:43 by cempassi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_TRACEROUTE_H
#define FT_TRACEROUTE_H
#include "libft.h"
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/ip_icmp.h>
#include <netinet/udp.h>
#include <stdint.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>

#define OPTSTR "hm:v"

#define OPT_H 0x0001
#define OPT_V 0x0002
#define OPT_M 0x0004

#define OPT_C_ERROR "-q [NUMBER]"
#define OPT_C_E_STR "invalid number of probes: "
#define OPT_M_ERROR "-m [NUMBER]"
#define OPT_M_E_STR "Invalid max TTL: "
#define OPT_S_ERROR "-s [NUMBER]"
#define OPT_S_E_STR "wrong total length, 88 instead of"

#define MAX_TTL 1

#define DEFAULT_HOPS 10
#define DEFAULT_TIMEOUT 5
#define DEFAULT_PROBES 3
#define DEFAULT_PAYLOAD "42"
#define DEFAULT_PAYLOAD_LEN 50
#define DEFAULT_SRC_PORT 3490
#define DEFAULT_DST_PORT 33434

#define MAX_PAYLOAD_SIZE 64
#define UDP_HEADER_LEN 8
#define IP_HEADER_LEN 20
#define PACKET_HEADER_LEN UDP_HEADER_LEN + IP_HEADER_LEN
#define TIME_DATA sizeof(t_time)

typedef struct addrinfo    t_addrinfo;
typedef struct iphdr       t_ipheader;
typedef struct udphdr      t_udpheader;
typedef struct icmphdr     t_icmpheader;
typedef struct sockaddr_in t_socket;

typedef struct s_time
{
    struct timeval sent;
    struct timeval recv;
} t_time;

typedef struct s_packet
{
    t_ipheader  ipheader;
    t_udpheader udpheader;
    char        payload[];
} t_packet;

typedef struct s_response
{
    t_ipheader   outer;
    t_icmpheader header;
    t_ipheader   inner;
    t_udpheader  udp;
    char         payload[];
} t_response;

typedef struct s_traceroute
{
    int      output;
    int      input;
    uint32_t hops;
    uint32_t timeout;
    uint8_t  probes;
    int16_t  exit;
    char *   name;
    char *   host;
    uint32_t options;
    uint32_t packet_size;
    uint32_t payload_size;
    char *   payload;
    t_socket dest;
    uint16_t dest_port;
    t_socket current;
    int      finished;
} t_traceroute;

int init_prgm(t_traceroute *traceroute, int ac, char **av);
int init_option(t_traceroute *traceroute, int ac, char **av);

int generate_payload(t_traceroute *traceroute, t_packet *template);

int resolve_dst(t_traceroute *traceroute);
int resolve_response(t_traceroute *traceroute, t_response *response,
                     t_socket *address);

void setup_udphdr(t_traceroute *traceroute, t_packet *packet, uint16_t port);
void setup_iphdr(t_traceroute *traceroute, t_packet *packet, uint8_t ttl,
                 uint16_t seq);

uint16_t checksum(void *addr, int count);
void     get_time(t_traceroute *traceroute, void *time);

int select_packets(t_traceroute *traceroute, t_time *time);
int send_packets(t_traceroute *traceroute, t_packet *packet, t_time *time);

/*
*****************************************************
********************* DISPLAY ***********************
*****************************************************
*/

void display_help(char *name);
void display_start(t_traceroute *traceroute);
void display_icmppacket(struct s_response *packet);
void display_response(t_response *packet, int recieved);
void display_ipheader(t_ipheader *header);
void display_udpheader(t_udpheader *header, char *payload);
void print_bytes(int bytes, void *msg);

#endif
