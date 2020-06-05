
#ifndef SIK_DUZE_COMMON_H
#define SIK_DUZE_COMMON_H

#include <unordered_map>
#include <string>
#include <cstring>

#include <cstddef>
#include <memory>
#include <netinet/in.h>

#include <unistd.h>
#include <fcntl.h>

#include <sys/time.h>

static bool elo_elo_cos_signal_wlanelo = false;

class parameters {
public:
    parameters(int argc, char *argv[]);
protected:
    std::unordered_map<int, std::string> m;
    bool check_number(std::string &nr);
private:
    int check_flag(char* f);
};

class local_message {
public:
    enum msg_type {DISCOVER = 1, IAM = 2, KEEPALIVE = 3, AUDIO = 4, METADATA = 6};
    local_message() {};
    local_message(size_t size);
    msg_type get_type();
    uint16_t get_length();
    void set_type(uint16_t);
    void set_length(uint16_t);
    uint8_t* get_buff_pointer();
    uint8_t* get_body_pointer();
    struct sockaddr addr;
    size_t size;
    std::string get_body_as_string();

private:
    std::shared_ptr<uint8_t[]> buffer;
};

class local_interface {
public:
    local_interface();
    ~local_interface();
    bool receive_message(std::shared_ptr<local_message> msg);
    void send_message(std::shared_ptr<local_message> msg);
    int sock;
};

static size_t sockaddr_to_num(const struct sockaddr s) {
    size_t r = ((uint64_t)s.sa_family<<48) +
               ((uint64_t)s.sa_data[0]<<40) +
               ((uint64_t)s.sa_data[1]<<32) +
               ((uint64_t)s.sa_data[2]<<24) +
               ((uint64_t)s.sa_data[3]<<16) +
               ((uint64_t)s.sa_data[4]<<8) +
               ((uint64_t)s.sa_data[5]);
    return r;
}

static bool operator==(const struct sockaddr& lhs, const struct sockaddr& rhs) {
    return sockaddr_to_num(lhs) == sockaddr_to_num(rhs);
}

namespace std {
    template<> struct hash<struct sockaddr>
    {
        std::size_t operator()(struct sockaddr const& s) const noexcept
        {
            return sockaddr_to_num(s);
        }
    };
}

class no_time_exception: public std::exception {
public:
    no_time_exception() {};
    virtual const char* what() const throw() {
        return "Couldn't get time";
    }
};

static size_t get_wall_time(){
    struct timeval time;
    if (gettimeofday(&time, nullptr)){
        throw no_time_exception();
    }

    return (size_t)time.tv_sec * 1000 + (size_t)(time.tv_usec / 1000);
}

class sigint_exception : public std::exception {
public:
    const char * what () const throw () {
        return "sigint";
    }
};

static void handleSigInt(__attribute__((unused)) int s) {
    elo_elo_cos_signal_wlanelo = true;
}

static void check_sigint() {
    if(elo_elo_cos_signal_wlanelo) throw sigint_exception();
}

#endif //SIK_DUZE_COMMON_H

