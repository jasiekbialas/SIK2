//
// Created by jasiek on 29.05.20.
//

#include <netinet/in.h>
#include <arpa/inet.h>
#include <zconf.h>
#include "local_interface.h"



local_interface::local_interface(std::string port) {

    /* argumenty wywołania programu */
    in_port_t local_port =(in_port_t)stol(port);

    /* zmienne i struktury opisujące gniazda */
    struct sockaddr_in local_address;

    /* otwarcie gniazda */
    sock = socket(AF_INET, SOCK_DGRAM, 0);


    if (sock < 0)
        throw "socket";

    /* ustawienie adresu i portu lokalnego */
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(local_port);

    if (bind(sock, (struct sockaddr *)&local_address, sizeof local_address) < 0)
        throw "bind";
}

local_interface::local_interface(std::string addr, std::string port) {
    in_port_t local_port;
    multicast = true;

    /* zmienne i struktury opisujące gniazda */
    struct sockaddr_in local_address;

    local_port = (in_port_t)stol(port);

    /* otwarcie gniazda */
    sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock < 0)
        throw "socket";

    /* podłączenie do grupy rozsyłania (ang. multicast) */
     ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);

     if (inet_aton(addr.c_str(), &ip_mreq.imr_multiaddr) == 0) {
       throw "inet_aton - invalid multicast address";
     }

     if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&ip_mreq, sizeof ip_mreq) < 0)
       throw "setsockopt";

    /* ustawienie adresu i portu lokalnego */
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(local_port);

    if (bind(sock, (struct sockaddr *)&local_address, sizeof local_address) < 0)
        throw "bind";
}

local_interface::~local_interface() {
    if(multicast) {
         if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*)&ip_mreq, sizeof ip_mreq) < 0)
               throw "setsockopt";
    }
    close(sock);
}

std::optional<struct local_interface::msg> local_interface::receive_message() {
    struct sockaddr src_addr;
    socklen_t addrlen;

    struct msg_head{
        uint16_t type;
        uint16_t length;

    } msg_h;

    ssize_t rcv_len;
    rcv_len = recvfrom(sock, &msg_h, sizeof(msg_head), 0, &src_addr, &addrlen);
    if(rcv_len < 0) {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            return std::nullopt;
        throw "local read";
    }
    return (msg){(enum msg_type) msg_h.type, src_addr};
}




