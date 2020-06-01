//
// Created by jasiek on 31.05.20.
//

#ifndef ZADANIE2_LOCAL_SERVER_H
#define ZADANIE2_LOCAL_SERVER_H


#include "local_interface.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

class local_server: public local_interface {
public:
    local_server(std::string port);
    local_server(std::string port, std::string multi);
    ~local_server();
private:
    sockaddr_in local_address;
    struct ip_mreq ip_mreq;
    bool multicast = false;
};


#endif //ZADANIE2_LOCAL_SERVER_H
