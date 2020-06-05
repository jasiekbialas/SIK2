
#include "client.h"

#define TTL_VALUE     69

client_interface::client_interface(std::string address, std::string port_local): local_interface() {

    /* uaktywnienie rozgłaszania (ang. broadcast) */
    int optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof optval) < 0)
        throw std::runtime_error("setsockopt broadcast");

    /* ustawienie TTL dla datagramów rozsyłanych do grupy */
    optval = TTL_VALUE;
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&optval, sizeof optval) < 0)
        throw std::runtime_error("setsockopt multicast ttl");


    struct addrinfo addr_hints;
    struct addrinfo *addr_result;

    addr_hints.ai_family = AF_INET;
    addr_hints.ai_socktype = SOCK_DGRAM;
    addr_hints.ai_protocol = IPPROTO_UDP;
    addr_hints.ai_flags = 0;
    addr_hints.ai_addrlen = 0;
    addr_hints.ai_addr = NULL;
    addr_hints.ai_canonname = NULL;
    addr_hints.ai_next = NULL;

    if (getaddrinfo(address.c_str(), NULL, &addr_hints, &addr_result) != 0)
        throw std::runtime_error("get addr info");

    struct sockaddr_in addr_in;
    addr_in.sin_family = AF_INET;
    addr_in.sin_addr.s_addr = ((struct sockaddr_in *) (addr_result->ai_addr))->sin_addr.s_addr;
    addr_in.sin_port = htons((uint16_t) stol(port_local));

    freeaddrinfo(addr_result);

    broadcast_addr = *((struct sockaddr *) &addr_in);
};
