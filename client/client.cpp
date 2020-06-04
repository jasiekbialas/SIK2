
#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <poll.h>

#include "client.h"

int main(int argc, char *argv[]) {
    try {

        client_parameters params(argc, argv);

        auto cl_int = std::make_shared<client_interface>(params.get_host(), params.get_local_port());

        telnet_listen_sock telnet_listen_sock(params.get_telnet_port());

        struct pollfd fds[2];

        auto buffer_msg = std::make_shared<local_message>(MAX_MESSAGE_SIZE);

        int client_timeout = params.get_timeout();

        bool eloelo = false;
        int timeout = std::min(KEEPALIVE_TIMEOUT, client_timeout);

        while(!eloelo) {
            try {
                auto tel_int = std::make_shared<telnet_interface>(telnet_listen_sock.sock);
                auto usr_int = std::make_shared<user_interface>(tel_int);
                client_manager cl_manager(cl_int, usr_int, client_timeout);

                fds[0] = {tel_int->msg_sock, POLLIN, 0};
                fds[1] = {-1, POLLIN, 0};

                user_interface::event event;

                while (!eloelo) {

                    if (poll(fds, 2, timeout) == -1) {
                        eloelo = true;
                        break;
                    }

                    if (fds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                        throw telnet_interface::telnet_error();
                    }

                    if (fds[1].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                        throw std::runtime_error("server disconnected");
                    }

                    event = usr_int->handle_input();
//                    std::cout<<event<<std::endl;

                    switch (event) {
                        case user_interface::event::SEARCH:
                            fds[1].fd = cl_int->sock;
                            cl_manager.send_discover_to_all();
                            break;
                        case user_interface::event::NEW_PLAYING:
                            cl_manager.change_playing(usr_int->new_playing());
                            break;
                        case user_interface::event::END:
                            eloelo = true;
                            break;
                    }
                    if(eloelo) break;

                    cl_manager.handle_one_message(buffer_msg);

                    timeout = std::min(cl_manager.handle_time(), KEEPALIVE_TIMEOUT);

                    fds[0].revents = 0;
                    fds[1].revents = 0;
                }
            } catch (telnet_interface::telnet_error &e) {
                std::cout<<"TELNET "<<e.what()<<std::endl;
            }
        }

    } catch (std::exception &e) {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }

    return 0;
}
