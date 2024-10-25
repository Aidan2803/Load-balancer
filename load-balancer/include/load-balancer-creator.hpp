#ifndef LOAD_BALANCER_CREATOR_HPP
#define LOAD_BALANCER_CREATOR_HPP

#include <memory>

#include "load-balancer-mods.hpp"
#include "load-balancer-pseudo.hpp"
#include "load-balancer-roundrobin.hpp"

class LoadBalancerCreator {
  public:
    std::shared_ptr<LoadBalancerServerInterface> CreateLoadBalancer(LoadBalancerMods load_balancer_mode) {
        if (load_balancer_mode == LoadBalancerMods::Pseudo) {
            return std::make_unique<LoadBalancerServerPseudo>("[LoadBalancerPseudo]");
        } else if (load_balancer_mode == LoadBalancerMods::RoundRobin) {
            return std::make_unique<LoadBalancerServerRoundRobin>("[LoadBalancerRoundRobin]");
        }
    }
};

#endif