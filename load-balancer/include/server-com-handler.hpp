#ifndef SERVER_COM_HANDLER_HPP
#define SERVER_COM_HANDLER_HPP

#include <iostream>
#include <string>
#include <memory>

#include "SocketWrapper.hpp"

class ServerComHandler{
    public:
    ServerComHandler();
    ~ServerComHandler();
    void EstablishConnectionWithRemoteServer(std::pair<const char*, const char*> server);
    void SendRequestToRemoteServer(std::string &request_buffer) const;
    std::string ReceiveResponseFromRemoteServer() const;
    
    private:
    std::unique_ptr<SocketWrapper> server_socket_wrapper_;
};

#endif