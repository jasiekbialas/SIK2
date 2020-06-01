//
// Created by jasiek on 29.05.20.
//

#ifndef ZADANIE2_LOCAL_INTERFACE_H
#define ZADANIE2_LOCAL_INTERFACE_H


#include <string>
#include <optional>
#include <unistd.h>
#include <fcntl.h>

#include "local_message.h"


class local_interface {
public:
    bool receive_message(std::shared_ptr<local_message> msg);
    void send_message(local_message msg);
    ~local_interface();
protected:
    local_interface();
    int sock;
};

#endif //ZADANIE2_LOCAL_INTERFACE_H
