//
// Created by jasiek on 27.05.20.
//


#include "parameters.h"
#include "shoutcast_request.h"

std::string shoutcast_request::to_string() {
    std::string request;
    request.append("GET ");
    request.append(params.get_resource());
    request.append(" HTTP/1.0\r\n");
    request.append("Host: ");
    request.append(params.get_host());
    request.append("\r\n");
    if(params.get_meta()) {
        request.append("Icy-MetaData: 1\r\n");
    }
    request.append("\r\n");

    return request;
};