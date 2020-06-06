
#include "common.h"


static bool elo_elo_cos_signal_wlanelo = false;

void check_sigint() {
    if(elo_elo_cos_signal_wlanelo) throw sigint_exception();
}

bool get_elo_elo_cos_signal_walenlo() {
    return elo_elo_cos_signal_wlanelo;
}

void handle_sig_int(__attribute__((unused)) int s) {
    elo_elo_cos_signal_wlanelo = true;
}

const char * sigint_exception::what () const throw () {
    return "sigint";
}

size_t get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time, nullptr)){
        throw std::runtime_error("time");
    }

    return (size_t)time.tv_sec * 1000 + (size_t)(time.tv_usec / 1000);
}

bool operator==(const struct sockaddr& lhs, const struct sockaddr& rhs) {
    return sockaddr_to_num(lhs) == sockaddr_to_num(rhs);
}

size_t sockaddr_to_num(const struct sockaddr s) {
    size_t r = ((uint64_t)s.sa_family<<48) +
               ((uint64_t)s.sa_data[0]<<40) +
               ((uint64_t)s.sa_data[1]<<32) +
               ((uint64_t)s.sa_data[2]<<24) +
               ((uint64_t)s.sa_data[3]<<16) +
               ((uint64_t)s.sa_data[4]<<8) +
               ((uint64_t)s.sa_data[5]);
    return r;
}