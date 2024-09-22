#ifndef LOAD_BALANCER_INTERFACE
#define LOAD_BALANCER_INTERFACE

#include <exception>
#include <iostream>
#include <memory>
#include <vector>

#include "SocketWrapper.hpp"
#include "client-com-handler.hpp"
#include "server-com-handler.hpp"
#include "server-info.hpp"

class LoadBalancerServerInterface {
  public:
    LoadBalancerServerInterface();

    virtual void LoadBalancing() = 0;
    virtual void DEBUG_PushServers() = 0;

    void StartLoadBalancerServer();

  protected:
    void GetServersInfo();  // TODO: to be implemeted [need to get information about servers (ip, port, etc) from a JSON
                            // file], shall have an implementation in abstract class
    void ChechServersList();

    void DEBUG_PushTestServer();
    void DEBUG_PushFiveTestServers();

  protected:
    std::vector<ServerInfo> servers_;

    const char* port_{"8090"};
    int backlog_size_;

    std::unique_ptr<SocketWrapper> load_balancer_socket_wrapper_;
    ServerComHandler server_com_handler_;
    ClientComHandler client_com_handler_;
};

#endif