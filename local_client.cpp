//
// Created by jasiek on 31.05.20.
//




#include <iostream>
#include "local_client.h"
#define TTL_VALUE     4

local_client::local_client(): local_interface() {
    /* uaktywnienie rozgłaszania (ang. broadcast) */
    int optval = 1;
    if (setsockopt(sock, SOL_SOCKET, SO_BROADCAST, (void*)&optval, sizeof optval) < 0)
        throw std::runtime_error("setsockopt broadcast");

    /* ustawienie TTL dla datagramów rozsyłanych do grupy */
    optval = TTL_VALUE;
    if (setsockopt(sock, IPPROTO_IP, IP_MULTICAST_TTL, (void*)&optval, sizeof optval) < 0)
        throw std::runtime_error("setsockopt multicast ttl");

};
