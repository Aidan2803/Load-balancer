#ifndef SERVER_INFO_HPP
#define SERVER_INFO_HPP

struct ServerInfo {
    ServerInfo(const std::string ip, const std::string port, bool is_available)
        : ip_{ip}, port_{port}, is_available_{is_available} {}
    const std::string ip_;
    const std::string port_;
    bool is_available_;
};

#endif