//
// Created by jasiek on 30.05.20.
//

#ifndef ZADANIE2_LOCAL_MESSAGE_H
#define ZADANIE2_LOCAL_MESSAGE_H


#include <cstddef>
#include <memory>
#include <sys/socket.h>

class local_message {
public:
    enum msg_type {DISCOVER = 1, IAM = 2, KEEPALIVE = 3, AUDIO = 4, METADATA = 5};
    local_message() {};
    local_message(size_t size);
    msg_type get_type();
    uint32_t get_length();
    void set_type(uint16_t);
    void set_length(uint16_t);
    uint8_t* get_buff_pointer();
    uint8_t* get_body_pointer();
    struct sockaddr addr;
    size_t size;
private:
    std::shared_ptr<uint8_t[]> buffer;
};


#endif //ZADANIE2_LOCAL_MESSAGE_H
