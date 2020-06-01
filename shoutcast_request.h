//
// Created by jasiek on 27.05.20.
//

#ifndef ZADANIE2_HTTP_REQUEST_H
#define ZADANIE2_HTTP_REQUEST_H

#include <iostream>
#include "parameters.h"


class shoutcast_request {
    parameters params;
public:
    shoutcast_request(parameters &params): params(params) {};
    std::string to_string();
};


#endif //ZADANIE2_HTTP_REQUEST_H
