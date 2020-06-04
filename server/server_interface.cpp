
#include "server.h"

server_interface::server_interface(std::string port):local_interface() {

    in_port_t local_port =(in_port_t)stol(port);

    /* ustawienie adresu i portu lokalnego */
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(local_port);

    if (bind(sock, (struct sockaddr *)&local_address, sizeof local_address) < 0)
        throw std::runtime_error("bind");

}

server_interface::server_interface(std::string port, std::string multi):local_interface() {
    in_port_t local_port =(in_port_t)stol(port);
    /* ustawienie adresu i portu lokalnego */
    local_address.sin_family = AF_INET;
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);
    local_address.sin_port = htons(local_port);

    /* podłączenie do grupy rozsyłania (ang. multicast) */
    ip_mreq.imr_interface.s_addr = htonl(INADDR_ANY);

    if (inet_aton(multi.c_str(), &ip_mreq.imr_multiaddr) == 0) {
        throw std::runtime_error("inet_aton - invalid multicast address");
    }

    if (setsockopt(sock, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&ip_mreq, sizeof ip_mreq) < 0)
        throw std::runtime_error("setsockopt add membership");

    fcntl(sock, F_SETFL, O_NONBLOCK);


    if (bind(sock, (struct sockaddr *)&local_address, sizeof local_address) < 0)
        throw std::runtime_error("bind");
}

server_interface::~server_interface(){
    if(multicast){
        if (setsockopt(sock, IPPROTO_IP, IP_DROP_MEMBERSHIP, (void*)&ip_mreq, sizeof ip_mreq) < 0)
            throw std::runtime_error("setsockopt drop memberhsip");
    }
};