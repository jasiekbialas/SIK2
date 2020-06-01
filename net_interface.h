//
// Created by jasiek on 28.05.20.
//

#ifndef ZADANIE2_NET_INTERFACE_H
#define ZADANIE2_NET_INTERFACE_H


#include <string>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <memory>

class net_interface {
public:
    net_interface(std::string addr,
            std::string port,
            unsigned long timeout);
    ~net_interface();
    void send_request(std::string request);
    std::string net_getline();
    size_t net_getchunk(uint8_t* buff, size_t size);
private:
    int sock;

};


#endif //ZADANIE2_NET_INTERFACE_H


