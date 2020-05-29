//
// Created by jasiek on 26.05.20.
//


#include <iostream>
#include <map>
#include <string>
#include <regex>
#include "parameters.h"
#include "net_interface.h"
#include "shoutcast_request.h"
#include "connection_manager.h"


int main(int argc, char *argv[]) {
//    std::cout << "RADIO PROXY" << std::endl;

    try {
        parameters params(argc, argv);
        net_interface interface(params.get_host(), params.get_port(), params.get_timeout());
        shoutcast_request request(params);
        connection_manager manager(interface);
        manager.start(request.to_string(), params.get_meta());


    } catch(char const* s) {
        std::cout<<"ERROR: "<<s<<std::endl;
    }

    return 0;
}

