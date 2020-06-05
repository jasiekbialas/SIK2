
#include <iostream>
#include <arpa/inet.h>
#include "common.h"

local_interface::local_interface() {
    /* otwarcie gniazda */
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
        throw std::runtime_error("socket");

    if(fcntl(sock, F_SETFL, O_NONBLOCK) == -1) {
        close(sock);
        throw std::runtime_error("fcntl");
    }
}

local_interface::~local_interface() {
    close(sock);
}

bool local_interface::receive_message(std::shared_ptr<local_message> msg) {
    socklen_t addrlen = sizeof(struct sockaddr);
    ssize_t rcv_len;
    rcv_len = recvfrom(sock, msg->get_buff_pointer(), msg->size, 0, &msg->addr, &addrlen);

    if(rcv_len < 0) {
        if(errno == EBADF && errno == ENOTSOCK)
            throw std::runtime_error("local read");

        return false;
    }
    return rcv_len == msg->get_length()+4;
}

void local_interface::send_message(std::shared_ptr<local_message> msg) {
    socklen_t socklen = sizeof(sockaddr);
//    std::cout<<"sending "<<msg->get_type()<<" to ";
//    printf("%s\n", inet_ntoa(((struct sockaddr_in *) &msg->addr)->sin_addr));
//    std::cout<<std::endl;
    if(sendto(sock, msg->get_buff_pointer(), msg->get_length()+4, 0, &msg->addr, socklen) < 0) {
        if (errno == EBADF && errno == ENOTSOCK)
            throw std::runtime_error("local send");
    }
};
