
#include "client.h"

client_manager::client_manager(
        std::shared_ptr<client_interface> in,
        std::shared_ptr<user_interface> user_interface,
        int t
        ){
    interface = in;
    usr_interface = user_interface;
    timeout = t;
}

void client_manager::send(sockaddr addr, local_message::msg_type type) {
    auto msg = local_message(4);
    msg.set_type(type);
    msg.set_length(0);
    msg.addr = addr;
    interface->send_message(std::make_shared<local_message>(msg));
    if(type == local_message::msg_type::KEEPALIVE) last_sent_keep_alive = get_wall_time();
}

bool client_manager::handle_one_message(std::shared_ptr<local_message> buffer_msg) {
    if(interface->receive_message(buffer_msg)) {
//        std::cout<<"handle: t:"<<buffer_msg->get_type()<<std::endl;
        std::string name, meta;
        switch(buffer_msg->get_type()) {
            case local_message::msg_type::IAM:
                name = buffer_msg->get_body_as_string();
//                std::cout<<"I AM: "<<name<<std::endl;
                usr_interface->add_server(std::make_pair(buffer_msg->addr, name));
                break;
            case local_message::msg_type::METADATA:
                if(last_heard_from_playing != 0 && now_playing == buffer_msg->addr) {
                    last_heard_from_playing = get_wall_time();
                    meta = buffer_msg->get_body_as_string();
                    usr_interface->add_meta(meta);
                }
                break;
            case local_message::msg_type::AUDIO:
                if(last_heard_from_playing != 0 && now_playing == buffer_msg->addr) {
                    last_heard_from_playing = get_wall_time();
                    write(STDOUT_FILENO, buffer_msg->get_body_pointer(), buffer_msg->get_length());
                }
                break;
        }

        return true;
    } else {
        return false;
    }
}

void client_manager::change_playing(struct sockaddr addr) {
    now_playing = addr;
    send(addr, local_message::msg_type::DISCOVER);
    send(now_playing, local_message::msg_type::KEEPALIVE);
    last_heard_from_playing = get_wall_time();
}

int client_manager::handle_time() {
    if(last_heard_from_playing != 0) {
        size_t current = get_wall_time();
        size_t time_elapsed = current - last_heard_from_playing;

        if(time_elapsed > timeout) {
            usr_interface->playing_gone();
            last_heard_from_playing = 0;
        }

        if(current - last_sent_keep_alive > KEEPALIVE_TIMEOUT) {
            send(now_playing, local_message::msg_type::KEEPALIVE);
        }

        return timeout - (int)time_elapsed;
    }
    return -1;
}

void client_manager::send_discover_to_all() {
//    std::cout<<"send"<<std::endl;
    send(interface->broadcast_addr, local_message::msg_type::DISCOVER);
}