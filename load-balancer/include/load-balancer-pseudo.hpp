#ifndef LOAD_BALANCER_PSEUDO
#define LOAD_BALANCER_PSEUDO

#include <poll.h>

#include "load-balancer-interface.hpp"

class LoadBalancerServerPseudo : public LoadBalancerServerInterface {
  public:
    LoadBalancerServerPseudo(const std::string &instance_name);
    virtual void LoadBalancing() override;
    virtual void HandleClient(ServerComHandler &server_com_handler,
                              std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper, ServerInfo &server) override;
    virtual void DEBUG_PushServers() override;
};

#endif