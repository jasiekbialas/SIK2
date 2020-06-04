
#include "server.h"

regexps::regexps():
    metaint("^icy-metaint: ?(0*[1-9][0-9]*)\r\n$",
            std::regex_constants::ECMAScript | std::regex_constants::icase),
    name("^icy-name: ?(.*)\r\n$",
            std::regex_constants::ECMAScript | std::regex_constants::icase),
    first_line("^(HTTP/1.[0-1]|ICY) ([1-5][0-9][0-9]) (.*)\r\n$",
            std::regex_constants::ECMAScript | std::regex_constants::icase)
{};

radio_manager::radio_manager(std::shared_ptr<radio_interface> in, int t): regexps() {
    interface = in;
    timeout = t;
}

bool radio_manager::manage_headers() {
    std::string line;
    bool meta_present = false;
    while(true) {
        line = interface -> net_getline();

        if(line == "\r\n") break;
        parse_one_header(line);
    }
    return meta_present;
}

void radio_manager::parse_one_header(std::string line) {
    std::smatch pieces_match;

    if(regex_match(line, pieces_match, regexps.metaint)) {
        metaint = std::stoul(pieces_match[1]);
    } else if(regex_match(line, pieces_match, regexps.name)) {
        name = pieces_match[1];
    }
}

bool radio_manager::get_message(std::shared_ptr<local_message> message) {
    size_t current_time = get_wall_time();
    if(current_time -last_read > timeout) {
        throw std::runtime_error("timeout");
    }
    last_read = current_time;

    size_t msg_size = MESSAGE_SIZE;
    local_message::msg_type msg_type = local_message::msg_type::AUDIO;

    if(metaint > 0) {
        if(metaint < msg_size) msg_size = metaint;

        if(metaint - received == 0) {
            uint8_t k;
            interface -> net_getchunk(&k, 1);
            msg_size = k*16;
            if(msg_size > 0) {
                last_metadata = "";
            }
            meta_left = msg_size;
            msg_type = local_message::msg_type::METADATA;
            received = 0;

        } else if(meta_left > 0) {
            msg_type = local_message::msg_type::METADATA;
            msg_size = meta_left;
        } else if(metaint - received < MESSAGE_SIZE) {
               msg_size = metaint - received;
        }
    }

    if(msg_size) {
        message -> set_type(msg_type);
        msg_size = interface -> net_getchunk(message -> get_body_pointer(), msg_size);
        message -> set_length(msg_size);
        if(metaint > 0) {
            if(msg_type == local_message::msg_type::AUDIO) received += msg_size;
            if(msg_type == local_message::msg_type::METADATA) {
                last_metadata.append(message->get_body_as_string());
                meta_left -= msg_size;
            }
        }
        return true;
    }
    return false;
}

void radio_manager::start(std::string request, bool asking_for_meta) {
    interface -> send_request(request);
    std::string first_line = interface->net_getline();

    if(!regex_match(first_line, regexps.first_line)) {
        throw std::runtime_error("wrong http response");
    }

    if(manage_headers() && !asking_for_meta) throw std::runtime_error("unwanted meta");

    interface->set_nonblock();
    last_read = get_wall_time();
}
