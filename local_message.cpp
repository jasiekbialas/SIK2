//
// Created by jasiek on 30.05.20.
//


#include <malloc.h>
#include "local_message.h"


local_message::local_message(size_t s): size(s) {
    buffer = std::shared_ptr<uint8_t[]> (new uint8_t[size]);
}

local_message::msg_type local_message::get_type() {
    return (msg_type)(((uint32_t)buffer.get()[1]<<8) + ((uint32_t)buffer.get()[0]));
}

uint32_t local_message::get_length() {
    return ((uint32_t)buffer.get()[3]<<8) + ((uint32_t)buffer.get()[2]);
}

void local_message::set_type(uint16_t t) {
    ((uint16_t*)buffer.get())[0] = t;
}

void local_message::set_length(uint16_t l) {
    ((uint16_t*)buffer.get())[1] = l;
}

uint8_t* local_message::get_buff_pointer() {
    return buffer.get();
}

uint8_t* local_message::get_body_pointer() {
    uint8_t* i = buffer.get();
    return i+4;
}