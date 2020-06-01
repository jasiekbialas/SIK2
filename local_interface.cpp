//
// Created by jasiek on 29.05.20.
//

#include <iostream>
#include "local_interface.h"

local_interface::local_interface() {

    /* otwarcie gniazda */
    sock = socket(AF_INET, SOCK_DGRAM, 0);

    if (sock < 0)
        throw std::runtime_error("socket");

//    if (bind(sock, (struct sockaddr *)&local_address, sizeof local_address) < 0)
//        throw std::runtime_error("bind");
}

local_interface::~local_interface() {
    close(sock);
}

bool local_interface::receive_message(std::shared_ptr<local_message> msg) {
    sockaddr src_addr;
    socklen_t addrlen = sizeof(struct sockaddr);

    ssize_t rcv_len;
    rcv_len = recvfrom(sock, msg->get_buff_pointer(), msg->size, 0, &msg->addr, &addrlen);

    if(rcv_len < 0) {
        if(errno == EAGAIN || errno == EWOULDBLOCK)
            return false;

        throw std::runtime_error("local read");
    }
    return true;
}

void local_interface::send_message(local_message msg) {
    socklen_t socklen = sizeof(sockaddr);
    if(sendto(sock, msg.get_buff_pointer(), msg.size, 0, &msg.addr, socklen) < 0){
        std::cout<<"send errno: "<<errno<<std::endl;
    };
};