
#include "client.h"

#define HOST (int)'h'
#define TELNET_PORT (int)'p'
#define TIMEOUT (int)'t'
#define LOCAL_PORT (int)'P'

client_parameters::client_parameters(int argc, char *argv[]): parameters(argc, argv) {

    auto it = m.find(HOST);
    if(it == m.end()) throw std::runtime_error("No HOST argument");

    it = m.find(TELNET_PORT);
    if(it == m.end()) throw std::runtime_error("No PORT argument");
    if(!check_number(it->second)) throw std::runtime_error("PORT should be a number");

    it = m.find(LOCAL_PORT);
    if(it == m.end()) throw std::runtime_error("No PORT argument");
    if(!check_number(it->second)) throw std::runtime_error("PORT should be a number");

    it = m.find(TIMEOUT);
    if(it == m.end()) {
        m[TIMEOUT] = "5";
    } else {
        if(!check_number(it->second)) throw std::runtime_error("TIMEOUT should be a number");
    }

}


std::string client_parameters::get_host() {
    return m.find(HOST)->second;
}
std::string client_parameters::get_telnet_port() {
    return m.find(TELNET_PORT)->second;
}

int client_parameters::get_timeout() {
    return (int)stoul(m.find(TIMEOUT)->second)*1000;
}

std::string client_parameters::get_local_port() {
    return  m.find(LOCAL_PORT)->second;
}


