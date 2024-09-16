#ifndef LOAD_BALANCER
#define LOAD_BALANCER

#include <iostream>
#include <exception>
#include <vector>
#include <memory>

#include "SocketWrapper.hpp"
#include "server-com-handler.hpp"
#include "client-com-handler.hpp"

class LoadBalancerServer{
    public:
    LoadBalancerServer();
    void LoadBalancerProcess();
    private:
    void StartLBServer();
    void ServerLoop();

    void GetServersInfo(); // TODO: to be implemeted

    void DEBUG_PushTestServer();

    private:
    std::vector<std::pair<const char*, const char*>> servers_;

    const char* port_{"8090"};

    std::unique_ptr<SocketWrapper> load_balancer_socket_wrapper_;
    ServerComHandler server_com_handler_;
    ClientComHandler client_com_handler_;
};

#endif
