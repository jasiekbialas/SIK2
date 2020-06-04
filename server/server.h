
#ifndef SIK_DUZE_SERVER_H
#define SIK_DUZE_SERVER_H

#include <iostream>
#include <fcntl.h>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>

#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring>
#include <memory>

#include <regex>

#include <unordered_map>

#include "common/common.h"

#define MESSAGE_SIZE 4080
#define MAX_MESSAGE_SIZE 65000

class server_parameters : public parameters {
public:
    server_parameters(int argc, char *argv[]);
    std::string get_host();
    std::string get_resource();
    std::string get_port();
    bool get_meta();
    int get_timeout();
    std::optional<std::string> get_local_port();
    std::optional<std::string> get_multi();
    int get_local_timeout();
private:
    bool check_meta(std::string &m);
};

class shoutcast_request {
    server_parameters params;
public:
    shoutcast_request(server_parameters &params): params(params) {};
    std::string to_string();
};

class radio_interface {
public:
    radio_interface(std::string addr,
                  std::string port,
                  unsigned long timeout);
    ~radio_interface();
    void send_request(std::string request);
    void set_nonblock();
    std::string net_getline();
    size_t net_getchunk(uint8_t* buff, size_t size);
    int sock;
};

struct regexps {
    std::regex metaint;
    std::regex name;
    std::regex first_line;
    regexps();
};

class radio_manager {
public:
    radio_manager(std::shared_ptr<radio_interface> in, int timeout);
    void start(std::string request, bool read_meta);
    bool get_message(std::shared_ptr<local_message> message);
    std::string name = "no name";
private:
    std::shared_ptr<radio_interface> interface;
    struct regexps regexps;
    unsigned long metaint = 0;
    unsigned long meta_left = 0;
    unsigned long received = 0;

    std::string last_metadata = "";

    bool manage_headers();
    void parse_one_header(std::string line);

    int timeout;
    size_t last_read;
};

class server_interface: public local_interface {
public:
    server_interface(std::string port);
    server_interface(std::string port, std::string multi);
    ~server_interface();
private:
    sockaddr_in local_address;
    struct ip_mreq ip_mreq;
    bool multicast = false;
};

class server_manager {
public:
    server_manager(
            std::shared_ptr<server_interface> interface,
            std::string name, int timeout);

    void handle_all_messages();
    void send_out_message(std::shared_ptr<local_message>);

private:
    std::shared_ptr<server_interface> interface;
    std::unordered_map<struct sockaddr, size_t, std::hash<sockaddr>> client_map;
    bool handle_one_message();
    std::shared_ptr<local_message> iam_name;

    int timeout;
};

#endif //SIK_DUZE_SERVER_H
