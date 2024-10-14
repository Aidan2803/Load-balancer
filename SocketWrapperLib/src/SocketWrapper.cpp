#include "SocketWrapper.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

SocketWrapper::SocketWrapper(int fd) : fd_{fd} {
    spdlog::info("{} Constructing a socket...", instance_name_);
    if (fd == -1) {
        spdlog::critical("{} Invalid file descriptor given", instance_name_);
        throw std::invalid_argument("[SocketWrapper] Invalid file descriptor given");
    }

    int opt = 1;
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        spdlog::critical("{} Could not set socket options: {}", instance_name_, strerror(errno));
        throw std::runtime_error(std::string("[SocketWrapper] Could not set socket options: ") + strerror(errno));
        exit(EXIT_FAILURE);
    }
}

SocketWrapper::SocketWrapper(int domain, int type, int protocol) {
    spdlog::info("{} Constructing a socket...", instance_name_);
    fd_ = socket(domain, type, protocol);
    if (fd_ == -1) {
        spdlog::critical("{} Could not create a socket: {}", instance_name_, strerror(errno));
        throw std::runtime_error(std::string("[SocketWrapper]  Could not create a socket: ") + strerror(errno));
    }

    int opt = 1;
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        spdlog::critical("{} Could not set socket options: {}", instance_name_, strerror(errno));
        throw std::runtime_error(std::string("[SocketWrapper] Could not set socket options: ") + strerror(errno));
        exit(EXIT_FAILURE);
    }
}

SocketWrapper::SocketWrapper(SocketWrapper&& other) noexcept : fd_(other.fd_) { other.fd_ = -1; }

SocketWrapper& SocketWrapper::operator=(SocketWrapper&& other) noexcept {
    if (this != &other) {
        if (fd_ != -1) {
            close(fd_);
        }
        fd_ = other.fd_;
        other.fd_ = -1;
    }
    return *this;
}

int SocketWrapper::GetSocketFileDescriptor() const { return fd_; }

SocketWrapper::~SocketWrapper() {
    if (fd_ != -1) {
        spdlog::info("{}  Closing a socket for {}", instance_name_, fd_);
        close(fd_);
    }
}
