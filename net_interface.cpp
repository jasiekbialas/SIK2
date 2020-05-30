//
// Created by jasiek on 28.05.20.
//


#include <iostream>
#include "net_interface.h"

net_interface::net_interface(std::string addr, std::string port, unsigned long timeout) {
    int err;
    struct addrinfo addr_hints;
    struct addrinfo *addr_result;
    struct timeval read_timeout;
    read_timeout.tv_sec = timeout;
    read_timeout.tv_usec = 0;

    memset(&addr_hints, 0, sizeof(struct addrinfo));

    addr_hints.ai_family = AF_INET; // IPv4
    addr_hints.ai_socktype = SOCK_STREAM;
    addr_hints.ai_protocol = IPPROTO_TCP;

    err = getaddrinfo(addr.c_str(), port.c_str(), &addr_hints, &addr_result);

    if (err != 0) { // other error (host not found, etc.)
        throw "getaddrinfo";
    }

    // initialize socket according to getaddrinfo results
    sock = socket(addr_result->ai_family, addr_result->ai_socktype, addr_result->ai_protocol);


    if (sock < 0) {
        freeaddrinfo(addr_result);
        throw "socket error";
    }

    // connect socket to the server
    if (connect(sock, addr_result->ai_addr, addr_result->ai_addrlen) < 0) {
        freeaddrinfo(addr_result);
        throw "connect error";
    }

    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (void *)&read_timeout,
               sizeof(read_timeout));

    freeaddrinfo(addr_result);
}

net_interface::~net_interface() {
    close(sock);
}

void net_interface::send_request(std::string request) {
    ssize_t sent = 0, sent_total = 0;

    while (sent_total < request.size()) {
        sent = write(sock, request.c_str() + sent_total, request.size() - sent_total);
        if (sent < 0) {
            throw "partial / failed write";
        }
        sent_total += sent;
    };
}

std::string net_interface::net_getline() {
    std::string r;
    char c;
    int len;
    bool last_was_r = false;
    while(true) {
        len = (int) read(sock, &c, 1);
        if(len != 1) {
            throw "partial / failed read";
        }
        r.push_back(c);
        if(c == '\n' && last_was_r) return r;
        last_was_r = c == '\r';
    }
}

size_t net_interface::net_getchunk(uint8_t* buff, size_t size) {
    if(size > BUFF_SIZE) size = BUFF_SIZE;
    ssize_t received = 0, received_total = 0;

    while (received_total < size) {
        received = read(sock, buff + received_total, size - received_total);
        if (received < 0) {
            throw "partial / failed read";
        } else if (received == 0) {
            throw "stream closed";
        }
        received_total += received;
    };
    return (size_t)received_total;
}
