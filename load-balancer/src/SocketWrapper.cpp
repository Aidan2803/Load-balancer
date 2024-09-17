#include "SocketWrapper.hpp"

#include <stdexcept>
#include <string>

SocketWrapper::SocketWrapper(int fd) : fd_{fd} {
    if (fd == -1) {
        throw std::invalid_argument("Invalid file descriptor given");
    }
}

SocketWrapper::SocketWrapper(int domain, int type, int protocol) {
    fd_ = socket(domain, type, protocol);
    if (fd_ == -1) {
        throw std::runtime_error(std::string("Could not create a socket: ") + strerror(errno));
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
        close(fd_);
    }
}
