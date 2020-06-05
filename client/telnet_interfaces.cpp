
#include "client.h"

static const unsigned char do_linemode[3] = {255, 253, 34};
static const unsigned char linemode_options[7] = {255, 250, 34, 1, 0, 255, 240};
static const unsigned char will_echo[3] = {255, 251, 1};
static const unsigned char invisible_cursor[] = { 27, '[', '?', '2', '5', 'l'};

telnet_interface::telnet_interface(int sock) {
    struct sockaddr_in client_address;
    socklen_t client_address_len;
    client_address_len = sizeof(client_address);

    msg_sock = accept(sock, (struct sockaddr *) &client_address, &client_address_len);

    if (msg_sock < 0) {
        throw telnet_error();
    }

    if(fcntl(msg_sock, F_SETFL, O_NONBLOCK) == -1) {
        close(msg_sock);
        throw std::runtime_error("fcntl");
    };
}

ssize_t telnet_interface::send(const unsigned char* msg) {
    return write(msg_sock, msg, sizeof(msg));
}

void telnet_interface::configure_telnet() {
    bool err = false;

    err = err || send(do_linemode) <= 0;

    err = err || send(linemode_options) <= 0;
    err = err || send(will_echo) <= 0;
    err = err || send(invisible_cursor) <= 0;

    if(err) {
        throw telnet_error();
    }
}

telnet_interface::~telnet_interface() {
  close(msg_sock);
}

ssize_t telnet_interface::send(std::string msg) {
    return write(msg_sock, msg.c_str(), msg.size());
}

bool telnet_interface::read_one(uint8_t &one) {
    if(read(msg_sock, &one, 1) <= 0) {
        if(errno == EWOULDBLOCK || errno == EAGAIN)
            return false;
        throw telnet_error();
    }
    return true;
}

telnet_interface::input telnet_interface::get_input() {
    uint8_t one;

    if(!read_one(one)) return OTHER;

    if(one == 27) {
        while(read_one(one)) {
            if(one != 27) break;
        }

        if(one == 91) {
            if(!read_one(one)) return OTHER;
            if(one == 65) return UP;
            if(one == 66) return DOWN;
        }
    }

    if(one == 13) {
        if(!read_one(one)) return OTHER;
        if(one == 0) return CONFIRM;
    }

    return OTHER;
}


telnet_listen_sock::telnet_listen_sock(std::string port) {
    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        throw std::runtime_error("sock");
    }
    struct sockaddr_in server_address;

    server_address.sin_family = AF_INET; // IPv4
    server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
    server_address.sin_port = htons((uint16_t)stol(port)); // listening on port PORT_NUM

    if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
        close(sock);
        throw std::runtime_error("bind listen");
    }

    if (listen(sock, 1) < 0) {
        close(sock);
        throw std::runtime_error("listen");
    }
}

telnet_listen_sock::~telnet_listen_sock() {
    close(sock);
}