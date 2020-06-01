//
// Created by jasiek on 30.05.20.
//

#include <iostream>
#include "local_client.h"
#include "local_message.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <vector>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"


static double get_wall_time(){
    struct timeval time;
    gettimeofday(&time, nullptr);
    return (double)time.tv_sec + (double)time.tv_usec * .000001;
}

int main(int argc, char *argv[]) {
    try {

        int sock = socket(PF_INET, SOCK_STREAM, 0);
        if(sock < 0) {
            throw std::runtime_error("sock");
        }
        struct sockaddr_in server_address;
        server_address.sin_family = AF_INET; // IPv4
        server_address.sin_addr.s_addr = htonl(INADDR_ANY); // listening on all interfaces
        server_address.sin_port = htons(10002); // listening on port PORT_NUM

        if (bind(sock, (struct sockaddr *) &server_address, sizeof(server_address)) < 0) {
            throw std::runtime_error("bind");
        }
        if (listen(sock, 5) < 0) {
            throw std::runtime_error("listen");
        }

        struct sockaddr_in client_address;
        socklen_t client_address_len;
        client_address_len = sizeof(client_address);

        int msg_sock = accept(sock, (struct sockaddr *) &client_address, &client_address_len);
        if (msg_sock < 0) {
            throw std::runtime_error("accept");
        }


        char buffer[20];
        size_t snd_len;


        unsigned char conf_telnet[3] = {255, 253, 34};
        int len = 3;
        write(msg_sock, conf_telnet, len);
        len = 7;
        unsigned char conf_telnet2[7] = {255, 250, 34, 1, 0, 255, 240};
        write(msg_sock, conf_telnet2, len);

        unsigned char conf_telnet25[3] = {255, 251, 1};
        len = 3;
        write(msg_sock, conf_telnet25, len);



        unsigned char invisible_cursor[] = {27, '[', '?', '2', '5', 'l'};
        write(msg_sock, invisible_cursor, sizeof(invisible_cursor));

        unsigned char clear[] = {27, '[', 'H', 27, '[', '0', 'J'};

//        = "\u001b[2K"
//        write(msg_sock, conf_telnet3, len);
//
        char msg[5];
        msg[0] = 'a';
        msg[1] = 'a';
        msg[2] = 'a';
        msg[3] = 'g';
        msg[4] = 'z';

        char nl[0];
//        = '\n';
        nl[0] = 10;
        nl[1] = 13;


        char cr = 13;


        size_t len_t;
        do {

            len_t = read(msg_sock, buffer, sizeof(buffer));
            write(msg_sock, clear, sizeof(clear));
//            write(msg_sock, &cr, 1);
//            write(msg_sock, conf_telnet3, len);

            for(int i = 0; i < len_t; i++){
                printf("%d ", buffer[i]);
            }
            if(len_t == 3 && buffer[0] == 27 && buffer[1] == 91) {
                if(buffer[2] == 65 && msg[2] < 'z') msg[2]++;
                if(buffer[2] == 66 && msg[2] > 'a') msg[2]--;
            }
            printf("\n");



//            write(msg_sock, &nl, 2);
            snd_len = write(msg_sock, msg, 5);
            write(msg_sock, &nl, 2);
            snd_len = write(msg_sock, msg, 5);

//            a++;
//            snd_len = write(msg_sock, &a, 1);
//            a--;


        } while (len_t > 0);

        close(sock);




//        local_client client;
//        in_port_t remote_port;
//
//
//        remote_port = (in_port_t)atoi("10001");
//        local_message m(4);
//
//        m.set_type(local_message::msg_type::DISCOVER);
//        m.set_length(0);
//
//        struct sockaddr_in remote_address;
//
//        remote_address.sin_family = AF_INET;
//        remote_address.sin_port = htons(remote_port);
//
//        if (inet_aton("255.255.255.255", &remote_address.sin_addr) == 0) {
//            fprintf(stderr, "ERROR: inet_aton - invalid multicast address\n");
//            exit(EXIT_FAILURE);
//        }
//
//        struct sockaddr* a;
//        a = (struct sockaddr *)&remote_address;
//        m.addr = *a;
//
//
//
//        client.send_message(m);
//        sleep(1);
//        client.send_message(m);
//        m.set_type(local_message::msg_type::KEEPALIVE);
//
//        auto msg = std::make_shared<local_message>(65536);
//
//        double time = get_wall_time();
//        double last = time;
//
//        while(true){
//
//            time = get_wall_time();
//            if(time - last > 3.5) {
//                client.send_message(m);
//                last = time;
//            }
//
//            if(client.receive_message(msg)) {
//                if(msg->get_type() == local_message::msg_type::AUDIO) {
//                    write(STDOUT_FILENO, msg->get_body_pointer(), msg->get_length());
//                } else if (msg->get_type() == local_message::msg_type::METADATA) {
//                    write(STDERR_FILENO, msg->get_body_pointer(), msg->get_length());
//                }
//            }
//        }

    } catch(std::exception &e) {
        std::cout<<"ERROR: "<<e.what()<<std::endl;
    }

    return 0;
}
#pragma clang diagnostic pop