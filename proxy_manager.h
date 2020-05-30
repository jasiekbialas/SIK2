//
// Created by jasiek on 30.05.20.
//

#ifndef ZADANIE2_PROXY_MANAGER_H
#define ZADANIE2_PROXY_MANAGER_H


#include <sys/socket.h>
#include <unordered_map>
#include <string>
#include <iostream>
#include <cstring>
#include <unordered_map>
#include <sys/time.h>
#include "local_interface.h"

static size_t sockaddr_to_num(const sockaddr s) {
    uint64_t *r;
    r = (uint64_t*)&s;
    std::size_t c = *r;
    return c;
}

bool operator==(const sockaddr& lhs, const sockaddr& rhs) {
    return sockaddr_to_num(lhs) == sockaddr_to_num(rhs);
}

namespace std {
    template<> struct hash<struct sockaddr>
    {
        std::size_t operator()(struct sockaddr const& s) const noexcept
        {
            return sockaddr_to_num(s);
        }
    };
}

class proxy_manager {
public:
    proxy_manager(local_interface interface);
    void handle_all_messages();
private:
    enum client_status {POTENTIAL_CLIENT, CLIENT};
    struct client_data {
        enum client_status status;
        double timestamp;
    };
    local_interface interface;
    std::unordered_map<sockaddr, client_data, std::hash<sockaddr>> client_map;
    double start_time;
    bool handle_one_message();

    static class no_time_exception: public std::exception
    {
        virtual const char* what() const throw()
        {
            return "Couldn't get time";
        }
    } time_exception;

    static double get_wall_time(){
        struct timeval time;
        if (gettimeofday(&time, nullptr)){
            throw time_exception;
        }
        
        return (double)time.tv_sec + (double)time.tv_usec * .000001;
    }

};




#endif //ZADANIE2_PROXY_MANAGER_H
