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
#include <netinet/in.h>
#include <memory>

#include "local_server.h"

static size_t sockaddr_to_num(const struct sockaddr s) {
    size_t r = ((uint64_t)s.sa_family<<48) +
            ((uint64_t)s.sa_data[0]<<40) +
            ((uint64_t)s.sa_data[1]<<32) +
            ((uint64_t)s.sa_data[2]<<24) +
            ((uint64_t)s.sa_data[3]<<16) +
            ((uint64_t)s.sa_data[4]<<8) +
            ((uint64_t)s.sa_data[5]);
    return r;
}

static bool operator==(const struct sockaddr& lhs, const struct sockaddr& rhs) {
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

class no_time_exception: public std::exception {
public:
    no_time_exception() {};
    virtual const char* what() const throw() {
        return "Couldn't get time";
    }
};

class proxy_manager {
public:
    proxy_manager(std::shared_ptr<local_server> interface, size_t timeout);
    void handle_all_messages();
    void send_out_message(local_message);

private:
    size_t timeout;
    enum client_status {POTENTIAL_CLIENT, CLIENT};
    struct client_data {
        enum client_status status;
        double timestamp;
    };
    std::shared_ptr<local_server> interface;
    std::unordered_map<struct sockaddr, client_data, std::hash<sockaddr>> client_map;
    double start_time;
    bool handle_one_message();



    static double get_wall_time(){
        struct timeval time;
        if (gettimeofday(&time, nullptr)){
            no_time_exception e;
            throw e;
        }

        return (double)time.tv_sec + (double)time.tv_usec * .000001;
    }

};




#endif //ZADANIE2_PROXY_MANAGER_H
