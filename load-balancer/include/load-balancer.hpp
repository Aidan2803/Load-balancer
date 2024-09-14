#include <iostream>
#include <exception>
#include <vector>
#include <memory>

#include "SocketWrapper.hpp"


class LoadBalancerServer{
    public:
    LoadBalancerServer();
    void LoadBalancerProcess();
    private:
    void StartLBServer();
    void ServerLoop();

    SocketWrapper&& AcceptClient() const;
    char* RecieveRequestFromClient(SocketWrapper&& client_socket_wrapper) const;
    SocketWrapper EstablishConnectionWithRemoteServer();
    void SendRequestToRemoteServer() const;
    std::string ReceiveResponseFromRemoteServer() const;
    void SendResponseToClient() const;


    void GetServersInfo(); // TODO: to be implemeted

    void DEBUG_PushTestServer();

    private:
    std::vector<std::pair<const char*, const char*>> servers_;

    const char* port_{"8090"};

    std::unique_ptr<SocketWrapper> server_socket_;
};
