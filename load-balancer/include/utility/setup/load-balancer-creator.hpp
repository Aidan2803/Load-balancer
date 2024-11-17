#ifndef LOAD_BALANCER_CREATOR_HPP
#define LOAD_BALANCER_CREATOR_HPP

#include <memory>

#include "load-balancer-mods.hpp"
#include "balancer/load-balancer-pseudo.hpp"
#include "balancer/load-balancer-roundrobin.hpp"

class LoadBalancerCreator {
  public:
    std::shared_ptr<LoadBalancerServerInterface> CreateLoadBalancer(LoadBalancerMods load_balancer_mode, const std::string &ip, const std::string &port) {
        spdlog::debug("ip {} port {}", ip, port);
        if (load_balancer_mode == LoadBalancerMods::Pseudo) {
            return std::make_unique<LoadBalancerServerPseudo>("[LoadBalancerPseudo]", ip, port);
        } else if (load_balancer_mode == LoadBalancerMods::RoundRobin) {
            return std::make_unique<LoadBalancerServerRoundRobin>("[LoadBalancerRoundRobin]", ip, port);
        }
    }
};

#endif