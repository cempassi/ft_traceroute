#include "ft_traceroute.h"
#include "memory.h"
#include <arpa/inet.h>
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

int resolve_node(t_traceroute *traceroute, t_socket *recv)
{
    int   error;
    char  node[125];
    char *ip;

    // Get Host
    // Display host address
    if (traceroute->current.sin_addr.s_addr == recv->sin_addr.s_addr)
    {
        return (0);
    }
    recv->sin_family = AF_INET;
    recv->sin_port = 0;
    if ((error = getnameinfo((struct sockaddr *)recv, sizeof(struct sockaddr),
                             node, 125, NULL, 0, 0))
        < 0)
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
    else
    {
        ip = inet_ntoa(recv->sin_addr);
        printf("%s (%s) ", node, ip);
    }
    traceroute->current = *recv;
    return (0);
}
