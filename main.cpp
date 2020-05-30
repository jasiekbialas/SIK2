//
// Created by jasiek on 26.05.20.
//


#include <iostream>
#include <map>
#include <string>
#include <regex>
#include <arpa/inet.h>
#include "parameters.h"
#include "net_interface.h"
#include "shoutcast_request.h"
#include "connection_manager.h"
#include "local_interface.h"




#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"



int main(int argc, char *argv[]) {
//    std::cout << "RADIO PROXY" << std::endl;

    try {
        parameters params(argc, argv);
        local_interface l_interface(params.get_port());


        while(true) {



        }

        std::cout<<"done"<<std::endl;

    } catch(char const* s) {
        std::cout<<"ERROR: "<<s<<std::endl;
    }

    return 0;
}


#pragma clang diagnostic pop