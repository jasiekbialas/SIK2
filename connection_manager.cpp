//
// Created by jasiek on 28.05.20.
//


#include "connection_manager.h"

regexps::regexps():
    metaint("^icy-metaint: ?([1-9][0-9]*)\r\n$",
            std::regex_constants::ECMAScript | std::regex_constants::icase),
    br("^icy-br: ?([1-9][0-9]*)\r\n$",
            std::regex_constants::ECMAScript | std::regex_constants::icase),
    first_line("^(HTTP/1.[0-1]|ICY) ([1-5][0-9][0-9]) (.*)$",
            std::regex_constants::ECMAScript | std::regex_constants::icase)
{};

connection_manager::connection_manager(std::shared_ptr<net_interface> in): regexps() {
    interface = in;
}

bool connection_manager::manage_headers() {
    std::string line;
    struct header h;
    bool meta_present = false;
    while(true) {
        line = interface -> net_getline();
        if(line == "\r\n") break;
//        std::cout<<line;
        h = parse_one_header(line);
        if(h.type == header::METAINT) {
            metaint = h.arg;
            meta_present = true;
        } else if(h.type == header::BR) {
            br = h.arg;
        }
    }
    return meta_present;
}

struct header connection_manager::parse_one_header(std::string line) {
    std::smatch pieces_match;
    struct header h;

    if(regex_match(line, pieces_match, regexps.metaint)) {
        h.arg = std::stoul(pieces_match[1]);
        h.type = header::METAINT;
    } else if(regex_match(line, pieces_match, regexps.br)) {
        h.arg = std::stoul(pieces_match[1]);
        h.type = header::BR;
    } else {
        h.type = header::OTHER;
    }
    return h;
}

local_message connection_manager::get_audio() {
    local_message message(metaint+4);
    message.set_type(local_message::msg_type::AUDIO);
    message.set_length(metaint);
    size_t l = interface -> net_getchunk(message.get_body_pointer(), metaint);
    return message;
}

local_message connection_manager::get_meta(){
    uint8_t k;
    interface -> net_getchunk(&k, 1);
    local_message message(k*16 + 4);
    message.set_length(k*16);
    message.set_type(local_message::msg_type::METADATA);
    interface -> net_getchunk(message.get_body_pointer(), k*16);

    return message;
}

void connection_manager::start(std::string request, bool asking_for_meta) {
    interface -> send_request(request);
    meta_present = manage_headers();
    if(!asking_for_meta && meta_present) throw std::runtime_error("unwanted meta");
}


