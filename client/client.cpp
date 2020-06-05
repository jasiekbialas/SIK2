
#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <poll.h>

#include "client.h"

int main(int argc, char *argv[]) {
    signal(SIGINT, handleSigInt);
    try {

        client_parameters params(argc, argv);
        auto client_int = std::make_shared<client_interface>(params.get_host(), params.get_local_port());
        telnet_listen_sock telnet_l_sock(params.get_telnet_port());

        struct pollfd fds[3];
        fds[0] = {telnet_l_sock.sock, POLLIN, 0};
        fds[1] = {-1, POLLIN, 0};
        fds[2] = {-1, POLLIN, 0};

        auto buffer_msg = std::make_shared<local_message>(MAX_MESSAGE_SIZE);
        int client_timeout = params.get_timeout();

        client_manager client_mgr(client_int, client_timeout);
        user_interface user_int;

        bool eloelo = false;
        int timeout = std::min(KEEPALIVE_TIMEOUT, client_timeout);

        std::string value;
        user_interface::event event;

        telnet_interface::input telnet_input;

        std::shared_ptr<telnet_interface> telnet_int = nullptr;

        while (!eloelo) {
            try {
                if (poll(fds, 3, timeout) == -1) {
                    throw std::runtime_error("poll error");
                }

                if (fds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                    throw std::runtime_error("telnet listen sock disconnected");
                }

                if (fds[1].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                    throw std::runtime_error("server disconnected");
                }

                if (fds[2].revents & (POLLERR | POLLHUP | POLLNVAL)) {
                    throw telnet_interface::telnet_error();
                }

                if (fds[0].revents & POLLIN) {
                    telnet_int = std::make_shared<telnet_interface>(telnet_l_sock.sock);
                    telnet_int->configure_telnet();
                    fds[2] = {telnet_int->msg_sock, POLLIN, 0};
                    fds[0].fd = -1;
                }

                telnet_input = telnet_interface::input::OTHER;
                if (telnet_int) telnet_input = telnet_int->get_input();

                event = user_int.handle_input(telnet_input);

                switch (event) {
                    case user_interface::event::SEARCH:
                        fds[1].fd = client_int->sock;
                        client_mgr.send_discover_to_all();
                        break;
                    case user_interface::event::NEW_PLAYING:
                        client_mgr.change_playing(user_int.new_playing());
                        break;
                    case user_interface::event::END:
                        eloelo = true;
                        break;
                }
                if (eloelo) break;

                client_mgr.handle_one_message(buffer_msg);

                if (client_mgr.last_event == client_manager::event::NEW_RADIO) {
                    user_int.add_server(std::make_pair(client_mgr.last_addr, client_mgr.last_string_value));
                } else if (client_mgr.last_event == client_manager::event::NEW_METADATA) {
                    user_int.add_meta(client_mgr.last_string_value);
                }

                timeout = std::min(client_mgr.handle_time(), KEEPALIVE_TIMEOUT);

                if (client_mgr.last_event == client_manager::event::PLAYING_GONE) {
                    user_int.playing_gone();
                }

                if (telnet_int) telnet_int->send(user_int.get_menu());

                check_sigint();

                fds[0].revents = 0;
                fds[1].revents = 0;
                fds[2].revents = 0;

            } catch (telnet_interface::telnet_error &e) {
                check_sigint();
                fds[2] = {-1, POLLIN, 0};
                fds[0].fd = telnet_l_sock.sock;
                telnet_int = nullptr;
            }
        }
    } catch (sigint_exception &e) {
        return 0;
    } catch (std::exception &e) {}
    if(elo_elo_cos_signal_wlanelo) return 0;
    return 1;
}
