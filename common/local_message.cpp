
#include "common.h"

local_message::local_message(size_t s): size(s) {
    buffer = std::shared_ptr<uint8_t[]> (new uint8_t[size]);
}

local_message::msg_type local_message::get_type() {
    return (msg_type)ntohs((((uint32_t)buffer.get()[1]<<8) + ((uint32_t)buffer.get()[0])));
}

uint16_t local_message::get_length() {
    return ntohs(((uint32_t)buffer.get()[3]<<8) + ((uint32_t)buffer.get()[2]));
}

void local_message::set_type(uint16_t t) {
    ((uint16_t*)buffer.get())[0] = htons(t);
}

void local_message::set_length(uint16_t l) {
    ((uint16_t*)buffer.get())[1] = htons(l);
}

uint8_t* local_message::get_buff_pointer() {
    return buffer.get();
}

uint8_t* local_message::get_body_pointer() {
    uint8_t* i = buffer.get();
    return i+4;
}

std::string local_message::get_body_as_string() {
    std::string s;
    for(int i = 0; i < get_length(); i++) {
        if(get_body_pointer()[i] == 0) break;
        s.push_back(get_body_pointer()[i]);
    }
    return s;
}

//void local_message::append(std::shared_ptr<local_message> msg {
//}