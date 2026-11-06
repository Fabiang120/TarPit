#include <iostream>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>

int main(){
  int status;
  char ipstr[INET6_ADDRSTRLEN];
  struct addrinfo hints{};
  hints.ai_family = AF_UNSPEC;
  hints.ai_socktype = SOCK_STREAM;

  struct addrinfo *linkedlist, *p;
  if((status= getaddrinfo("scanme.nmap.org", "22", &hints, &linkedlist)) != 0){
    // Research this
    fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    return 2;
  };
  for (p = linkedlist; p != nullptr; p = p->ai_next)
  {
    void *addr;
    const char *ipver = nullptr;
    struct sockaddr_in *ipv4;
    struct sockaddr_in6 *ipv6;
    if (p->ai_family == AF_INET)
    {
      ipv4 = (struct sockaddr_in *)p->ai_addr;
      addr = &(ipv4->sin_addr);
      ipver = "IPV4";
    }
    else if (p->ai_family == AF_INET6)
    {
      ipv6 = (struct sockaddr_in6 *)p->ai_addr;
      addr = &(ipv6->sin6_addr);
      ipver = "IPV6";
    }
    else
    {
      continue;
    }
    inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
    printf("%s: %s\n", ipver, ipstr);
  }
  freeaddrinfo(linkedlist);
  return 0;
}