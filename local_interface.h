//
// Created by jasiek on 29.05.20.
//

#ifndef ZADANIE2_LOCAL_INTERFACE_H
#define ZADANIE2_LOCAL_INTERFACE_H


#include <string>
#include <optional>

class local_interface {
public:
    enum msg_type {DISCOVER = 1, IAM = 2, KEEPALIVE = 3, AUDIO = 4, METADATA = 5};
    struct msg {
        msg_type type;
        sockaddr addr;
    };
    local_interface(std::string port);
    local_interface(std::string addr, std::string port);
    ~local_interface();
    std::optional<struct msg> receive_message();
private:
    bool multicast = false;
    struct ip_mreq ip_mreq;
    int sock;
};


#endif //ZADANIE2_LOCAL_INTERFACE_H
