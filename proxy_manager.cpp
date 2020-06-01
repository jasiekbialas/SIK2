//
// Created by jasiek on 30.05.20.
//

#include "proxy_manager.h"

proxy_manager::proxy_manager(std::shared_ptr<local_server> in, size_t t): timeout(t) {
    interface = in;
    start_time = get_wall_time();
};

bool proxy_manager::handle_one_message() {

    auto message = std::make_shared<local_message>(4);
    if(interface->receive_message(message)) {

        double timestamp = get_wall_time();
        auto iter = client_map.find(message->addr);
        if(iter == client_map.end()) {
            if(message->get_type() == local_message::msg_type::DISCOVER) {
                client_map[message->addr] = {POTENTIAL_CLIENT, timestamp};
            }
        }
        else if((iter -> second.status == CLIENT &&
                  message->get_type() == local_message::msg_type::KEEPALIVE) ||
                (iter -> second.status == POTENTIAL_CLIENT &&
                  message->get_type() == local_message::msg_type::DISCOVER)) {
            client_map[message->addr] = {
                    CLIENT, timestamp
            };
        }
        return true;
    } else {
        return false;
    }

}

void proxy_manager::handle_all_messages() {
    while(handle_one_message()){}

    double current_time = get_wall_time();
    auto it = client_map.begin();

    while(it != client_map.end()) {
        if(current_time - it->second.timestamp > timeout + 1) {
            std::cout<<it->second.timestamp-start_time<<" - "<<current_time-start_time<<std::endl;
            it = client_map.erase(it);
        } else {
            it++;
        }
    }
}

void proxy_manager::send_out_message(local_message msg) {

    double current_time = get_wall_time();

    for(auto [addr, client_data] : client_map) {
        if(client_data.status == CLIENT) {
            msg.addr = addr;
            interface->send_message(msg);
        }
    }
}