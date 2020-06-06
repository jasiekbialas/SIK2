
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

size_t sockaddr_to_num(const struct sockaddr s);

bool operator==(const struct sockaddr& lhs, const struct sockaddr& rhs);

namespace std {
    template<> struct hash<struct sockaddr>
    {
        std::size_t operator()(struct sockaddr const& s) const noexcept
        {
            return sockaddr_to_num(s);
        }
    };
}

size_t get_wall_time();

class sigint_exception : public std::exception {
public:
    const char * what () const throw ();
};

void handle_sig_int(__attribute__((unused)) int s);

void check_sigint();

bool get_elo_elo_cos_signal_walenlo();

#endif //SIK_DUZE_COMMON_H

