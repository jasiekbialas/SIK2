//
// Created by jasiek on 26.05.20.
//

#ifndef ZADANIE2_PARSER_H
#define ZADANIE2_PARSER_H

#include <unordered_map>
#include <string>
#include <iostream>
#include <cstring>

#define HOST (int)'h'
#define RESOURCE (int)'r'
#define PORT (int)'p'
#define META (int)'m'
#define TIMEOUT (int)'t'
#define LOCAL_PORT (int)'P'
#define MULTI (int)'B'
#define LOCAL_TIMEOUT (int)'T'


class parameters {
public:
    parameters(int argc, char *argv[]);
    std::string get_host();
    std::string get_resource();
    std::string get_port();
    bool get_meta();
    unsigned long get_timeout();
    std::string get_local_port();
//    bool get_multi();
    unsigned long get_local_timeout();
private:
    std::unordered_map<int, std::string> m;
    int check_flag(char* f);
    bool check_number(std::string &nr);
    bool check_meta(std::string &m) {
        return m == "yes" || m == "no";
    }
};


#endif //ZADANIE2_PARSER_H
