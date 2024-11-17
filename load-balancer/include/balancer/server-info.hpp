#ifndef SERVER_INFO_HPP
#define SERVER_INFO_HPP

#include <string>

struct ServerInfo {
    ServerInfo(const std::string ip, const std::string port, bool is_available)
        : ip_{ip}, port_{port}, is_available_{is_available} {}
    ServerInfo(const ServerInfo& other) = default;
    ServerInfo& operator=(const ServerInfo& other) {
        if (&other == this) {
            return *this;
        }

        this->ip_ = other.ip_;
        this->port_ = other.port_;
        this->is_available_ = other.is_available_;

        return *this;
    };
    std::string ip_;
    std::string port_;
    bool is_available_;
};

#endif