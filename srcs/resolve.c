#include "ft_traceroute.h"
#include "memory.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sysexits.h>

int resolve_dst(t_traceroute *traceroute)
{
    struct addrinfo *host;
    struct addrinfo  hints;
    int              error;

    host = NULL;
    ft_bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET;
    hints.ai_socktype = 0;
    hints.ai_flags = AI_ADDRCONFIG | AI_V4MAPPED;

    // Get Host
    if ((error = getaddrinfo(traceroute->host, "33434", &hints, &host)))
    {
        traceroute->exit = EX_NOHOST;
        if (error == EAI_NONAME)
            dprintf(STDERR_FILENO, "%s: cannot resolve %s: host unknown\n",
                    traceroute->name, traceroute->host);
        else
            dprintf(STDERR_FILENO, "%s: failed to reach host\n",
                    traceroute->name);
        return (-1);
    }

    // Store host address
    for (struct addrinfo *tmp = host; tmp != NULL; tmp = tmp->ai_next)
    {
        if (tmp->ai_family == AF_INET)
        {
            traceroute->dest.sin_addr.s_addr
                = ((struct sockaddr_in *)host->ai_addr)->sin_addr.s_addr;
            traceroute->dest.sin_family = (sa_family_t)host->ai_family;
            traceroute->dest.sin_port
                = ((struct sockaddr_in *)host->ai_addr)->sin_port;
            break;
        }
    }

    // Free list from getaddrinfo
    freeaddrinfo(host);
    return (0);
}

int resolve_response(t_traceroute *traceroute, t_response *response)
{
    t_socket address;
    int      error;
    char     current[INET_ADDRSTRLEN];
    char     dns[125];
    char     service[125];

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = response->outer.saddr;
    address.sin_port = response->udp.uh_dport;
    // Get Host
    if ((error
         = getnameinfo((struct sockaddr *)&address, sizeof(struct sockaddr),
                       dns, 125, service, 125, NI_NUMERICHOST | NI_NUMERICSERV))
        != 0)
    {
        traceroute->exit = EX_NOHOST;
        if (error == EAI_NONAME)
            dprintf(STDERR_FILENO, "%s: cannot resolve %s: host unknown - %s\n",
                    traceroute->name, traceroute->host, gai_strerror(error));
        else
            dprintf(STDERR_FILENO, "%s: failed to reach host - %s\n",
                    traceroute->name, gai_strerror(error));
        return (-1);
    }
    printf("%s ", dns);

    // Display host address
    t_socket *c_address = (t_socket *)&response->outer.saddr;
    if (traceroute->current.sin_addr.s_addr != c_address->sin_addr.s_addr)
    {
        inet_ntop(AF_INET, &address.sin_addr.s_addr, current, INET_ADDRSTRLEN);
        printf("(%s)", current);
        traceroute->current.sin_addr.s_addr = c_address->sin_addr.s_addr;
    }

    // Free list from getaddrinfo
    return (0);
}
