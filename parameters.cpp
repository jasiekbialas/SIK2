//
// Created by jasiek on 26.05.20.
//

#include "parameters.h"


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
        throw "Wrong number of arguments";
    }
    std::string param;
    m.emplace(META, "yes");
    m.emplace(TIMEOUT, "5");

    for(int i = 1; i < argc; i+=2) {
        param.assign(argv[i+1]);
        m.emplace(check_flag(argv[i]), param);
    }
    auto it = m.find(HOST);
    if(it == m.end()) throw "No HOST argument";
    it = m.find(RESOURCE);
    if(it == m.end()) throw "No RESURCE argument";
    it = m.find(PORT);
    if(it == m.end()) throw "No PORT argument";
    if(!check_number(it->second)) throw "PORT should be a number";
    it = m.find(TIMEOUT);
    if(!check_number(it->second)) throw "TIMEOUT should be a number";
    it = m.find(META);
    if(!check_meta(it->second)) throw "-m should be yes or no";

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