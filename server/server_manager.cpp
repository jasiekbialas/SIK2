
#include "server.h"

server_manager::server_manager(std::shared_ptr<server_interface> in, std::string name, int t): timeout(t) {
    interface = in;
    uint16_t msg_length = (uint16_t)(name.size() + 4 < MAX_MESSAGE_SIZE ? name.size()+4 : MAX_MESSAGE_SIZE);
    iam_name = std::make_shared<local_message>(msg_length);
    iam_name->set_type(local_message::msg_type::IAM);
    iam_name->set_length(msg_length-4);
    for(int i = 0; i<msg_length-4;i++) {
        iam_name->get_body_pointer()[i] = (uint8_t)name[i];
    }
};

bool server_manager::handle_one_message() {
    auto message = std::make_shared<local_message>(4);
    if(interface->receive_message(message)) {
        client_map[message->addr] =  get_wall_time();
        if(message->get_type() == local_message::msg_type::DISCOVER) {
            iam_name->addr = message->addr;
            interface->send_message(iam_name);
        }
        return true;
    }
    return false;
}

void server_manager::handle_all_messages() {
    while(handle_one_message()){}

    size_t current_time = get_wall_time();
    auto it = client_map.begin();

    while(it != client_map.end()) {
        if( ((int)(current_time - it->second)) > timeout) {
            it = client_map.erase(it);
        } else {
            it++;
        }
    }
}

void server_manager::send_out_message(std::shared_ptr<local_message> msg) {
    if(msg->get_type() == local_message::msg_type::METADATA) {
        std::cout<<"sending meta: "<<msg->get_body_as_string()<<std::endl;
    }
    for(auto pair : client_map) {
            msg->addr = pair.first;
            interface->send_message(msg);
    }
}
