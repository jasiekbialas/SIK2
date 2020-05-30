//
// Created by jasiek on 30.05.20.
//

#include "proxy_manager.h"




proxy_manager::proxy_manager(local_interface interface): interface(interface) {
    start_time = get_wall_time();
};


bool proxy_manager::handle_one_message() {
    std::optional<local_interface::msg> message;

    message = interface.receive_message();
    if(message) {
        double timestamp = get_wall_time();
        auto iter = client_map.find(message->addr);
        if(iter == client_map.end()) {
            if(message->type == local_interface::msg_type::DISCOVER) {
                client_map[message->addr] = {POTENTIAL_CLIENT, timestamp};
            }
        } else if((iter -> second.status == CLIENT &&
                  message->type == local_interface::msg_type::KEEPALIVE) ||
                (iter -> second.status == POTENTIAL_CLIENT &&
                  message->type == local_interface::msg_type::DISCOVER)) {
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
}