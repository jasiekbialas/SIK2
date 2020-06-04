
#include "server.h"

#define HOST (int)'h'
#define RESOURCE (int)'r'
#define PORT (int)'p'
#define META (int)'m'
#define TIMEOUT (int)'t'
#define LOCAL_PORT (int)'P'
#define MULTI (int)'B'
#define LOCAL_TIMEOUT (int)'T'

server_parameters::server_parameters(int argc, char *argv[]): parameters(argc, argv) {

    auto it = m.find(HOST);
    if(it == m.end()) throw std::runtime_error("No HOST argument");
    it = m.find(RESOURCE);
    if(it == m.end()) throw std::runtime_error("No RESURCE argument");
    it = m.find(PORT);
    if(it == m.end()) throw std::runtime_error("No PORT argument");
    if(!check_number(it->second)) throw std::runtime_error("PORT should be a number");


    it = m.find(TIMEOUT);
    if(it == m.end()) {
        m[TIMEOUT] = "5";
    } else {
        if(!check_number(it->second)) throw std::runtime_error("TIMEOUT should be a number");
    }

    it = m.find(META);
    if(it == m.end()) {
        m[META] = "no";
    } else {
        if(!check_meta(it->second)) throw std::runtime_error("-m should be yes or no");
    }

    it = m.find(LOCAL_TIMEOUT);
    if(it == m.end()) {
        m[LOCAL_TIMEOUT] = "5";
    } else {
        if(!check_number(it->second)) throw std::runtime_error("TIMEOUT should be a number");
    }

    it = m.find(LOCAL_PORT);
    if(it != m.end()) {
        if(!check_number(it->second)) throw std::runtime_error("PORT should be a number");
    }
}


std::string server_parameters::get_host() {
    return m.find(HOST)->second;
}

std::string server_parameters::get_resource() {
    return m.find(RESOURCE)->second;
}

std::string server_parameters::get_port() {
    return m.find(PORT)->second;
}

bool server_parameters::get_meta() {
    return m.find(META)->second == "yes";
}

int server_parameters::get_timeout() {
    return (int)stoul(m.find(TIMEOUT)->second)*1000;
}

std::optional<std::string> server_parameters::get_local_port() {
    auto it = m.find(LOCAL_PORT);
    if(it == m.end()) return std::nullopt;
    return it->second;
}

int server_parameters::get_local_timeout() {
    return (int)stoul(m.find(LOCAL_TIMEOUT)->second)*1000;
}

std::optional<std::string> server_parameters::get_multi() {
    auto it = m.find(MULTI);
    if(it == m.end()) return std::nullopt;
    return it->second;
}

bool server_parameters::check_meta(std::string &m) {
    return m == "yes" || m == "no";
}
