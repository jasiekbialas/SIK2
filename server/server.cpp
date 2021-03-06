
#include <iostream>
#include <string>
#include <memory>
#include <csignal>
#include <poll.h>

#include "server.h"

int main(int argc, char *argv[]) {
    signal(SIGINT, handle_sig_int);
    try {
        server_parameters params(argc, argv);
        shoutcast_request request(params);
        auto radio_int = std::make_shared<radio_interface>(params.get_host(), params.get_port(), params.get_timeout());
        radio_manager radio_mgr(radio_int, params.get_timeout());
        radio_mgr.start(request.to_string(), params.get_meta());

        auto buffer = std::make_shared<local_message>(MESSAGE_SIZE+4);

        auto local_port = params.get_local_port();
        struct pollfd fds[2];
        fds[0] = {radio_int->sock, POLLIN, 0};
        fds[1] = {-1, POLLIN, 0};

        if(local_port) {
            std::shared_ptr<server_interface> server_int;

            if(params.get_multi()) {
                server_int = std::make_shared<server_interface>(local_port.value(), params.get_multi().value());
            } else {
                server_int = std::make_shared<server_interface>(local_port.value());
            }
            fds[1] = {server_int->sock, POLLIN, 0};

            server_manager server_mgr(server_int, radio_mgr.name, params.get_local_timeout());

            bool send;

            while(poll(fds, 2, params.get_timeout()) >= 0) {

                send = radio_mgr.get_message(buffer);
                server_mgr.handle_all_messages();

                fds[0].revents = 0;
                fds[1].revents = 0;

                if(send) {
                    server_mgr.send_out_message(buffer);
                }

                check_sigint();
            }
        } else {
            while(poll(fds, 2, params.get_timeout()) > 0) {

                if(radio_mgr.get_message(buffer)) {
                    if(buffer->get_type() == local_message::msg_type::AUDIO) {
                        if(write(STDOUT_FILENO, buffer->get_body_pointer(), buffer->get_length()) <= 0) {
                            break;
                        };
                    }
                    if(buffer->get_type() == local_message::msg_type::METADATA) {
                        if(write(STDERR_FILENO, buffer->get_body_pointer(), buffer->get_length()) <=0 ) {
                            break;
                        };
                    }
                }

                check_sigint();
            }
        }
    } catch (sigint_exception &e) {
        return 0;
    } catch(std::exception &e) {
        std::cout<<"ERR: "<<e.what()<<std::endl;
    }

    if(get_elo_elo_cos_signal_walenlo()) return 0;
    return 1;
}
