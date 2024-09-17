#ifndef LOAD_BALANCER_PSEUDO
#define LOAD_BALANCER_PSEUDO

#include "load-balancer-interface.hpp"

class LoadBalancerServerPseudo : public LoadBalancerServerInterface {
  public:
    virtual void LoadBalancing() override;
};

#endif