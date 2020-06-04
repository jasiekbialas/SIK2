
#include "client.h"

static std::string szukaj = "Szukaj pośrednika";
static std::string koniec = "Koniec";
static std::string nl = "\n\r";
static std::string green = {27, '[', '3', '2', 'm'};
static std::string white = {27, '[', '3', '7', 'm'};
static std::string clear = {27, '[', 'H', 27, '[', '0', 'J', '\r'};

void user_interface::add_server(std::pair<struct sockaddr, std::string> server) {
    int i = 0;
    for(;i < options.size(); i++) {
        if(options[i].first == server.first) {
            options[i] = server;
            break;
        }
    }
    if(i == options.size()) options.push_back(server);
    send_menu_out();
}

void user_interface::playing_gone() {
    if(playing != -1) {
        options.erase(options.begin() + playing);
        playing = -1;
    }
}

void user_interface::add_meta(std::string m) {
    meta = m;
}

void user_interface::send_menu_out() {
    interface->send(clear);
    std::stringstream ss;

    if(selected == 0) ss << green << "\r";
    ss << szukaj;
    if(selected == 0) ss<<white;
    ss<<nl;

    for(int i = 0; i < options.size(); i++) {
        auto [addr, name] = options[i];
        bool mark = selected-1 == i;
        if(mark) ss << green << "\r";
        ss<<name;
        if(mark) ss<<white;
        ss<<nl;
    }

    if(selected == options.size() + 1) ss << green << "\r";;
    ss<<koniec;
    if(selected == options.size() + 1) ss<<white;;
    ss<<nl;
    interface->send(ss.str());

}

user_interface::event user_interface::handle_input() {

    auto r = NOTHING;
    auto input = interface->get_input();

    switch(input) {
        case telnet_interface::input::UP:
            if(selected > 0) selected--;
            break;
        case telnet_interface::input::DOWN:
            if(selected <= options.size()) selected++;
            break;
        case telnet_interface::input::CONFIRM:
            if(selected == 0) r = SEARCH;
            else if(selected <= options.size()) r = NEW_PLAYING;
            else r = END;
    }
    if(r == SEARCH) {
        std::pair<struct sockaddr, std::string> playa;
        if(playing >= 0) playa = options[playing];
        options.clear();
        if(playing >= 0) options.push_back(playa);
    }

    send_menu_out();
    return r;
}

struct sockaddr user_interface::new_playing() {
    playing = selected-1;
    return options[selected-1].first;
}

