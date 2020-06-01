//
// Created by jasiek on 28.05.20.
//

#ifndef ZADANIE2_RESPONSE_MANAGER_H
#define ZADANIE2_RESPONSE_MANAGER_H


#include <regex>
#include <iostream>
#include "net_interface.h"
#include "local_message.h"

struct header {
    enum h_type{METAINT, BR, OTHER} type;
    unsigned long arg;
};

struct regexps {
    std::regex metaint;
    std::regex br;
    std::regex first_line;
    regexps();
};

class connection_manager {
public:
    connection_manager(std::shared_ptr<net_interface> in);
    void start(std::string request, bool read_meta);
    local_message get_audio();
    local_message get_meta();
private:
    std::shared_ptr<net_interface> interface;
    struct regexps regexps;
    unsigned long metaint = 16384;
    unsigned long br = 0;
    bool meta_present;



    bool manage_headers();
    struct header parse_one_header(std::string line);

    void manage_stream();

};


#endif //ZADANIE2_RESPONSE_MANAGER_H
