#include "load-balancer-interface.hpp"

// TODO: create a parser based on the user input, 0 = json parser for now, we will see if there will be some more
// parsers
LoadBalancerServerInterface::LoadBalancerServerInterface(const std::string& instance_name)
    : servers_{},
      backlog_size_{10},
      load_balancer_socket_wrapper_{},
      server_com_handler_{},
      instance_name_{instance_name} {}

void LoadBalancerServerInterface::StartLoadBalancerServer() {
    if (servers_.empty()) {
        spdlog::critical("{} No available servers", instance_name_);
        throw std::runtime_error(instance_name_ + " No available servers");
    }

    thread_pool_ = std::make_unique<ThreadPool>(servers_.size());

    struct addrinfo hints {};
    struct addrinfo* address_info;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port_, &hints, &address_info) != 0) {
        spdlog::critical("{} Failed to get address info", instance_name_);
        throw std::runtime_error(instance_name_ + "Failed to get address info");
    }

    load_balancer_socket_wrapper_ =
        std::make_shared<SocketWrapper>(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);

    if (bind(load_balancer_socket_wrapper_->GetSocketFileDescriptor(), address_info->ai_addr,
             address_info->ai_addrlen) == -1) {
        spdlog::critical("{} Bind failed: {}", instance_name_, strerror(errno));
        throw std::runtime_error(std::string(instance_name_ + "Bind failed: ") + strerror(errno));
    }

    freeaddrinfo(address_info);

    if (listen(load_balancer_socket_wrapper_->GetSocketFileDescriptor(), backlog_size_) == -1) {
        spdlog::critical("{} Listen failed: {}", instance_name_, strerror(errno));
        throw std::runtime_error(instance_name_ + std::string("Listen failed: ") + strerror(errno));
    }
}

void LoadBalancerServerInterface::DEBUG_PushTestServer() { servers_.push_back(ServerInfo("127.0.0.1", "8080", false)); }

void LoadBalancerServerInterface::DEBUG_PushFiveTestServers() {
    servers_.push_back(ServerInfo("127.0.0.1", "8080", false));
    servers_.push_back(ServerInfo("127.0.0.1", "8081", false));
    servers_.push_back(ServerInfo("127.0.0.1", "8082", false));
    servers_.push_back(ServerInfo("127.0.0.1", "8083", false));
    servers_.push_back(ServerInfo("127.0.0.1", "8084", false));
}