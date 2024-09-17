#include "load-balancer-interface.hpp"

LoadBalancerServerInterface::LoadBalancerServerInterface() : server_com_handler_{}, client_com_handler_{} { StartLoadBalancerServer(); }

void LoadBalancerServerInterface::EstablishConnectionWithServers() {
    if (servers_.empty()) {
        throw std::runtime_error("[Load-Balancer] No servers were added to the server list, can not proceed!");
    }

    for (auto& server : servers_) {
        server_com_handler_.EstablishConnectionWithRemoteServer(server);
    }
}

void LoadBalancerServerInterface::StartLoadBalancerServer() {
    DEBUG_PushTestServer();

    struct addrinfo hints {};
    struct addrinfo* address_info;

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    if (getaddrinfo(NULL, port_, &hints, &address_info) != 0) {
        throw std::runtime_error("Failed to get address info");
    }

    load_balancer_socket_wrapper_ =
        std::make_unique<SocketWrapper>(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);

    if (bind(load_balancer_socket_wrapper_.get()->GetSocketFileDescriptor(), address_info->ai_addr,
             address_info->ai_addrlen) == -1) {
        throw std::runtime_error(std::string("Bind failed: ") + strerror(errno));
    }

    freeaddrinfo(address_info);

    if (listen(load_balancer_socket_wrapper_.get()->GetSocketFileDescriptor(), 10) == -1) {
        throw std::runtime_error(std::string("Listen failed: ") + strerror(errno));
    }
}

void LoadBalancerServerInterface::DEBUG_PushTestServer() { servers_.push_back(ServerInfo("127.0.0.1", "8080", false)); }