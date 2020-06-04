
#include "common.h"


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
    if(argc < 2) {
        throw std::runtime_error("Wrong number of arguments");
    }
    std::string param;

    for(int i = 1; i < argc; i+=2) {
        param.assign(argv[i+1]);
        m.emplace(check_flag(argv[i]), param);
    }
}