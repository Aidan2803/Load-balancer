#ifndef LOAD_BALANCER_PSEUDO
#define LOAD_BALANCER_PSEUDO

#include <poll.h>

#include "balancer/load-balancer-interface.hpp"

class LoadBalancerServerPseudo : public LoadBalancerServerInterface {
  public:
    LoadBalancerServerPseudo(const std::string &instance_name, const std::string& ip, const std::string &port);
    virtual void LoadBalancing() override;
    virtual void HandleClient(ServerComHandler &server_com_handler,
                              std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper,
                              std::vector<ServerInfo> &servers, int server_number) override;
    virtual void DEBUG_PushServers() override;
};

#endif