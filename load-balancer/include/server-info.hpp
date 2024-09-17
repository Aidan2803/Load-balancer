#ifndef SERVER_INFO_HPP
#define SERVER_INFO_HPP

struct ServerInfo {
    ServerInfo(const char* ip, const char* port, bool is_available)
        : ip_{ip}, port_{port}, is_available_{is_available} {}
    const char* ip_;
    const char* port_;
    bool is_available_;
};

#endif