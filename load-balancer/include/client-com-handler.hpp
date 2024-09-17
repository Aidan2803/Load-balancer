#ifndef CLIENT_COM_HANDLER_HPP
#define CLIENT_COM_HANDLER_HPP

#include <iostream>
#include <memory>
#include <string>

#include "SocketWrapper.hpp"

class ClientComHandler {
  public:
    ClientComHandler();
    ~ClientComHandler();
    void AcceptClient(std::unique_ptr<SocketWrapper> &load_balancer_socket_wrapper);
    std::string RecieveRequestFromClient();
    void SendResponseToClient(std::string &full_response) const;

  private:
    std::unique_ptr<SocketWrapper> client_socket_wrapper_;
};

#endif  // CLIENT_COM_HANDLER_HPP