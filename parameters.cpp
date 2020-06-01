//
// Created by jasiek on 26.05.20.
//

#include "parameters.h"

#define HOST (int)'h'
#define RESOURCE (int)'r'
#define PORT (int)'p'
#define META (int)'m'
#define TIMEOUT (int)'t'
#define LOCAL_PORT (int)'P'
#define MULTI (int)'B'
#define LOCAL_TIMEOUT (int)'T'


int parameters::check_flag(char* f) {
    if(strlen(f) != 2) return -1;
    if(f[0] != '-') return -1;
    return (int) f[1];
}

bool parameters::check_number(std::string &nr) {
    if(nr[0] == '0') return false;
    for(char l : nr) {
        if(!isdigit(l)) return false;
    }
    return true;
}

parameters::parameters(int argc, char *argv[]) {
    if(argc % 2 == 0 || argc < 7) {
        throw std::runtime_error("Wrong number of arguments");
    }
    std::string param;
    m.emplace(META, "yes");
    m.emplace(TIMEOUT, "5");

    for(int i = 1; i < argc; i+=2) {
        param.assign(argv[i+1]);
        m.emplace(check_flag(argv[i]), param);
    }
    auto it = m.find(HOST);
    if(it == m.end()) throw std::runtime_error("No HOST argument");
    it = m.find(RESOURCE);
    if(it == m.end()) throw std::runtime_error("No RESURCE argument");
    it = m.find(PORT);
    if(it == m.end()) throw std::runtime_error("No PORT argument");
    if(!check_number(it->second)) throw std::runtime_error("PORT should be a number");
    it = m.find(TIMEOUT);
    if(!check_number(it->second)) throw std::runtime_error("TIMEOUT should be a number");
    it = m.find(META);
    if(!check_meta(it->second)) throw std::runtime_error("-m should be yes or no");

}

std::string parameters::get_host() {
    return m.find(HOST)->second;
}
std::string parameters::get_resource() {
    return m.find(RESOURCE)->second;
}
std::string parameters::get_port() {
    return m.find(PORT)->second;
}
bool parameters::get_meta() {
    return m.find(META)->second == "yes";
}
unsigned long parameters::get_timeout() {
    return stoul(m.find(TIMEOUT)->second);
}
std::string parameters::get_local_port() {
    return m.find(LOCAL_PORT)->second;
}
unsigned long parameters::get_local_timeout() {
    return stoul(m.find(TIMEOUT)->second);
}