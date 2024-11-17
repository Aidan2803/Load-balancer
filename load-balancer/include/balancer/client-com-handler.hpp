#ifndef CLIENT_COM_HANDLER_HPP
#define CLIENT_COM_HANDLER_HPP

#include <iostream>
#include <memory>
#include <mutex>
#include <string>

#include "SocketWrapper.hpp"
#include "spdlog/spdlog.h"

class ClientComHandler {
  public:
    ClientComHandler();
    ~ClientComHandler() = default;
    void AcceptClient(std::shared_ptr<SocketWrapper> &load_balancer_socket_wrapper);
    std::string RecieveRequestFromClient();
    void SendResponseToClient(std::string &full_response);
    void CloseClientSocket();

  private:
    std::mutex client_mutex_;
    std::unique_ptr<SocketWrapper> client_socket_wrapper_;
    const std::string instance_name_;
};

#endif  // CLIENT_COM_HANDLER_HPP