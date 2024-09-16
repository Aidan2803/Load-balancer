#include "load-balancer.hpp"

LoadBalancerServer::LoadBalancerServer()
    : server_com_handler_{}, client_com_handler_{} {
    StartLBServer();
}

void LoadBalancerServer::StartLBServer() {
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

void LoadBalancerServer::LoadBalancerProcess() { ServerLoop(); }

void LoadBalancerServer::ServerLoop() {
    while (true) {
        try {
            // Accept a client
            client_com_handler_.AcceptClient(load_balancer_socket_wrapper_);
            // Recieve a request from a client

            std::string client_requst = client_com_handler_.RecieveRequestFromClient();
            // Resend request to a remote servers
            if (!servers_.empty()) {
                // Establish connection with the remote server
                server_com_handler_.EstablishConnectionWithRemoteServer(servers_[0]);

                // Resend request to the remote server
                server_com_handler_.SendRequestToRemoteServer(client_requst);

                // Receive response from a remote server
               std::string server_response = server_com_handler_.ReceiveResponseFromRemoteServer();

                // Send response back to client
                client_com_handler_.SendResponseToClient(server_response);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void LoadBalancerServer::DEBUG_PushTestServer() {
    servers_.push_back(std::pair<const char*, const char*>("127.0.0.1", "8080"));
}