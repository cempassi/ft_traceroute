#include "ft_traceroute.h"
#include "memory.h"
#include <stdio.h>
#include <sys/socket.h>
#include <sysexits.h>

int resolve_dst(t_traceroute *traceroute) {
  struct addrinfo *host;
  struct addrinfo hints;
  int error;

  host = NULL;
  ft_bzero(&hints, sizeof(struct addrinfo));
  hints.ai_family = AF_INET;
  hints.ai_socktype = 0;
  hints.ai_flags = AI_ADDRCONFIG | AI_V4MAPPED;

  // Get Host
  if ((error = getaddrinfo(traceroute->host, "33434", &hints, &host))) {
    traceroute->exit = EX_NOHOST;
    if (error == EAI_NONAME)
      dprintf(STDERR_FILENO, "%s: cannot resolve %s: host unknown\n",
              traceroute->name, traceroute->host);
    else
      dprintf(STDERR_FILENO, "%s: failed to reach host\n", traceroute->name);
    return (-1);
  }

  // Store host address
  for (struct addrinfo *tmp = host; tmp != NULL; tmp = tmp->ai_next) {
    if (tmp->ai_family == AF_INET) {
      traceroute->dest.sin_addr.s_addr =
          ((struct sockaddr_in *)host->ai_addr)->sin_addr.s_addr;
      traceroute->dest.sin_family = (sa_family_t)host->ai_family;
      break;
    }
  }

  // Free list from getaddrinfo
  freeaddrinfo(host);
  return (0);
}
