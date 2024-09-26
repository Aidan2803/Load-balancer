#ifndef LOAD_BALANCER_ROUNDROBIN
#define LOAD_BALANCER_ROUNDROBIN

#include "load-balancer-interface.hpp"

class LoadBalancerServerRoundRobin : public LoadBalancerServerInterface {
  public:
    virtual void HandleClient(ServerComHandler &server_com_handler, ClientComHandler &client_com_handler,
                              std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper, ServerInfo &server) override;
    virtual void LoadBalancing() override;
    virtual void DEBUG_PushServers() override;
};

#endif