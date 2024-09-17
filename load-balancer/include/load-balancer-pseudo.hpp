#ifndef LOAD_BALANCER
#define LOAD_BALANCER

#include "load-balancer-interface.hpp"

class LoadBalancerServerPseudo : public LoadBalancerServerInterface {
  public:
    virtual void LoadBalancing() override;
}

#endif