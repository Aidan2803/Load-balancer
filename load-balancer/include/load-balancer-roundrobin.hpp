#ifndef LOAD_BALANCER_ROUNDROBIN
#define LOAD_BALANCER_ROUNDROBIN

#include "load-balancer-interface.hpp"

class LoadBalancerServerRoundRobin : public LoadBalancerServerInterface {
  public:
    virtual void DEBUG_PushServers() override;
    virtual void LoadBalancing() override;
};

#endif