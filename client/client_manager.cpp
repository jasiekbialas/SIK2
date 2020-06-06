
#include "client.h"

client_manager::client_manager(
        std::shared_ptr<client_interface> in,
        int t
        ){
    interface = in;
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

void client_manager::handle_one_message(std::shared_ptr<local_message> buffer_msg) {
    last_event = NOTHING;
    if(interface->receive_message(buffer_msg)) {
        switch(buffer_msg->get_type()) {
            case local_message::msg_type::IAM:
                last_event = NEW_RADIO;
                last_string_value = buffer_msg->get_body_as_string();
                last_addr = buffer_msg->addr;
                break;

            case local_message::msg_type::METADATA:
                if(last_heard_from_playing != 0 && now_playing == buffer_msg->addr) {
                    last_heard_from_playing = get_wall_time();
                    last_event = NEW_METADATA;
                    last_string_value = buffer_msg->get_body_as_string();
                }
                break;

            case local_message::msg_type::AUDIO:
                if(last_heard_from_playing != 0 && now_playing == buffer_msg->addr) {
                    last_heard_from_playing = get_wall_time();
                    if(buffer_msg->get_length() > 0) {
                        if(write(STDOUT_FILENO, buffer_msg->get_body_pointer(), buffer_msg->get_length()) <= 0) {
//                            std::cout<<buffer_msg->get_length()<<std::endl;
//                            std::cout<<"errno: "<<errno<<std::endl;
                            throw std::runtime_error("write here");
                        }
                    }
                }
                break;
            case local_message::msg_type::DISCOVER:
            case local_message::msg_type::KEEPALIVE:
                break;
        }
    }
}

void client_manager::change_playing(struct sockaddr addr) {
    now_playing = addr;
    send(addr, local_message::msg_type::DISCOVER);
    send(now_playing, local_message::msg_type::KEEPALIVE);
    last_heard_from_playing = get_wall_time();
}

int client_manager::handle_time() {
    last_event = NOTHING;
    if(last_heard_from_playing != 0) {
        size_t current = get_wall_time();
        int time_elapsed = (int)(current - last_heard_from_playing);

        if(time_elapsed > timeout) {
            last_event = PLAYING_GONE;
            last_heard_from_playing = 0;
        }

        if(current - last_sent_keep_alive > KEEPALIVE_TIMEOUT) {
            send(now_playing, local_message::msg_type::KEEPALIVE);
        }

        return timeout - time_elapsed;
    }
    return -1;
}

void client_manager::send_discover_to_all() {
    send(interface->broadcast_addr, local_message::msg_type::DISCOVER);
}