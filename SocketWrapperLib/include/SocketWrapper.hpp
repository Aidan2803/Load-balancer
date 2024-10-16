#ifndef SOCKET_WRAPPER_HPP
#define SOCKET_WRAPPER_HPP

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cerrno>
#include <cstring>
#include "spdlog/spdlog.h"

class SocketWrapper {
  public:
    explicit SocketWrapper(int fd);

    SocketWrapper(int domain, int type, int protocol);

    SocketWrapper(SocketWrapper&& other) noexcept;

    SocketWrapper& operator=(SocketWrapper&& other) noexcept;

    SocketWrapper(const SocketWrapper&) = delete;
    SocketWrapper& operator=(const SocketWrapper&) = delete;

    int GetSocketFileDescriptor() const;

    ~SocketWrapper();

  private:
    const std::string instance_name_ = "[SocketWrapper]";
    int fd_{-1};
};

#endif