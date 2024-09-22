#include "SocketWrapper.hpp"

#include <iostream>
#include <stdexcept>
#include <string>

SocketWrapper::SocketWrapper(int fd) : fd_{fd} {
    std::cout << "[SocketWrapper] Constructing a socket...\n";
    if (fd == -1) {
        throw std::invalid_argument("[SocketWrapper] Invalid file descriptor given");
    }

    int opt = 1;
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        throw std::runtime_error(std::string("[SocketWrapper] Could not set socket options: ") + strerror(errno));
        exit(EXIT_FAILURE);
    }
}

SocketWrapper::SocketWrapper(int domain, int type, int protocol) {
    std::cout << "[SocketWrapper] Constructing a socket...\n";
    fd_ = socket(domain, type, protocol);
    if (fd_ == -1) {
        throw std::runtime_error(std::string("[SocketWrapper]  Could not create a socket: ") + strerror(errno));
    }

    int opt = 1;
    if (setsockopt(fd_, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
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
        std::cout << "[SocketWrapper] Closing a socket for " << fd_ << "\n";
        close(fd_);
    }
}
