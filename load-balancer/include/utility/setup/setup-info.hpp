#ifndef SETUP_INFO_HPP
#define SETUP_INFO_HPP

#include <vector>

#include "utility/setup/load-balancer-mods.hpp"
#include "balancer/server-info.hpp"

struct SetupInfo {
    SetupInfo()
        : servers_amount_{},
          server_ip_port_{},
          load_balancer_mode_{},
          load_balancer_ip_{},
          load_balancer_port_{},
          load_balancing_is_keep_alive{},
          load_balancer_backlog_size_{} {}
    int servers_amount_;
    std::vector<ServerInfo> server_ip_port_;
    LoadBalancerMods load_balancer_mode_;
    std::string load_balancer_ip_;
    std::string load_balancer_port_;
    bool load_balancing_is_keep_alive;
    int load_balancer_backlog_size_;
};

#endif