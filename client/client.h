
#ifndef SIK_DUZE_CLIENT_H
#define SIK_DUZE_CLIENT_H

#include <netinet/in.h>
#include <iostream>
#include <netdb.h>
#include <vector>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string>
#include <memory>
#include <sstream>

#include "common/common.h"

#define KEEPALIVE_TIMEOUT 3500
#define MAX_MESSAGE_SIZE 65500

class client_interface: public local_interface {
public:
    client_interface(std::string address, std::string port_local);
    struct sockaddr broadcast_addr;
};

class telnet_interface {
public:
    enum input {UP = 0, DOWN = 1, CONFIRM = 2, OTHER = 3};
    telnet_interface(int sock);
    ~telnet_interface();
    ssize_t send(std::string);
    ssize_t send(const unsigned char* msg);
    input get_input();
    bool read_one(uint8_t &one);
    int msg_sock;
    class telnet_error : public std::exception {
    public:
        const char * what () const throw () {
            return "telnet error";
        }
    };
};

class telnet_listen_sock {
public:
    telnet_listen_sock(std::string port);
    ~telnet_listen_sock();
    int sock;
};

class user_interface {
public:
    enum event {SEARCH, END, NEW_PLAYING, NOTHING};
    user_interface(std::shared_ptr<telnet_interface> in) {
        interface = in;
    };
    void add_server(std::pair<struct sockaddr, std::string> server);
    void playing_gone();
    void add_meta(std::string meta);
    event handle_input();
    struct sockaddr new_playing();
private:
    std::vector<std::pair<struct sockaddr, std::string>> options;
    void send_menu_out();
    std::string meta = "";
    int playing = -1;
    int selected = 0;
    std::shared_ptr<telnet_interface> interface;
};


class client_manager {
public:
    client_manager(std::shared_ptr<client_interface> interface, std::shared_ptr<user_interface> user_interface, int timeout);
    void send_discover_to_all();
    bool handle_one_message(std::shared_ptr<local_message> buffer_msg);
    void change_playing(struct sockaddr addr);
    int handle_time();
private:
    void send(sockaddr addr, local_message::msg_type type);

    std::shared_ptr<client_interface> interface;
    std::shared_ptr<user_interface> usr_interface;
    struct sockaddr now_playing;

    int timeout;
    size_t last_heard_from_playing = 0;
    size_t last_sent_keep_alive;
};

class client_parameters: public parameters {
public:
    client_parameters(int argc, char *argv[]);
    std::string get_host();
    std::string get_telnet_port();
    std::string get_local_port();
    int get_timeout();
};

#endif //SIK_DUZE_CLIENT_H
