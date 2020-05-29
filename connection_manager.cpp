//
// Created by jasiek on 28.05.20.
//


#include <iostream>
#include "connection_manager.h"

regexps::regexps():
    metaint("^icy-metaint: ?([1-9][0-9]*)\r\n$",
            std::regex_constants::ECMAScript | std::regex_constants::icase),
    br("^icy-br: ?([1-9][0-9]*)\r\n$",
            std::regex_constants::ECMAScript | std::regex_constants::icase),
    first_line("^(HTTP/1.[0-1]|ICY) ([1-5][0-9][0-9]) (.*)$",
            std::regex_constants::ECMAScript | std::regex_constants::icase)
{};


connection_manager::connection_manager(net_interface interface):
interface(interface), regexps() {};


bool connection_manager::manage_headers() {
    std::string line;
    struct header h;
    bool meta_present = false;
    while(true) {
        line = interface.net_getline();
        if(line == "\r\n") break;
//        std::cout<<line;
        h = parse_one_header(line);
        if(h.type == header::METAINT) {
            std::cout<<line;
            metaint = h.arg;
            meta_present = true;
        } else if(h.type == header::BR) {
            std::cout<<line;
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

void connection_manager::manage_stream() {
    uint8_t buff[metaint];
    while(true) {
        interface.net_getchunk(buff, metaint);
        write(STDOUT_FILENO, buff, metaint);
        if(meta_present) {
            interface.net_getchunk(buff, 1);
            uint8_t k = buff[0];
            interface.net_getchunk(buff, k*16);
            write(STDERR_FILENO, buff, k*16);
        }
    }
}

void connection_manager::start(std::string request, bool asking_for_meta) {
    interface.send_request(request);
    meta_present = manage_headers();
    if(!asking_for_meta && meta_present) throw "unwanted meta";
    manage_stream();

}



//void connection_manager::get_meta() {}