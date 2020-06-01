//
// Created by jasiek on 26.05.20.
//


#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <arpa/inet.h>
#include <memory>

#include "parameters.h"
#include "net_interface.h"
#include "shoutcast_request.h"
#include "connection_manager.h"
#include "local_interface.h"
#include "local_server.h"
#include "local_message.h"
#include "proxy_manager.h"


#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"



int main(int argc, char *argv[]) {
//    std::cout << "SERVER PROXY" << std::endl;

    try {
        parameters params(argc, argv);
        shoutcast_request request(params);
        auto n_interface = std::make_shared<net_interface>(params.get_host(), params.get_port(), params.get_timeout());
        connection_manager radio_manager(n_interface);
        radio_manager.start(request.to_string(), params.get_meta());

        auto l_server = std::make_shared<local_server>(params.get_local_port());

        proxy_manager local_manager(l_server, params.get_timeout());

        local_message audio, meta;

        while(true){

            audio = radio_manager.get_audio();
            meta = radio_manager.get_meta();


            if(meta.get_length() > 0) {
                local_manager.send_out_message(meta);
            }

            local_manager.handle_all_messages();
            local_manager.send_out_message(audio);
        }

    } catch(std::exception &e) {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }

    return 0;
}


#pragma clang diagnostic pop