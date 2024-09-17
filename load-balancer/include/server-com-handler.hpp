#ifndef SERVER_COM_HANDLER_HPP
#define SERVER_COM_HANDLER_HPP

#include <iostream>
#include <memory>
#include <string>

#include "SocketWrapper.hpp"
#include "server-info.hpp"

class ServerComHandler {
  public:
    ServerComHandler();
    ~ServerComHandler();
    void EstablishConnectionWithRemoteServer(ServerInfo &server);
    void SendRequestToRemoteServer(std::string &request_buffer) const;
    std::string ReceiveResponseFromRemoteServer() const;

  private:
    std::unique_ptr<SocketWrapper> server_socket_wrapper_;
};

#endif