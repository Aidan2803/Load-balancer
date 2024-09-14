#include "load-balancer.hpp"

LoadBalancerServer::LoadBalancerServer() { StartLBServer(); }

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

    server_socket_ =
        std::make_unique<SocketWrapper>(address_info->ai_family, address_info->ai_socktype, address_info->ai_protocol);

    if (bind(server_socket_.get()->GetSocketFileDescriptor(), address_info->ai_addr, address_info->ai_addrlen) == -1) {
        throw std::runtime_error(std::string("Bind failed: ") + strerror(errno));
    }

    freeaddrinfo(address_info);

    if (listen(server_socket_.get()->GetSocketFileDescriptor(), 10) == -1) {
        throw std::runtime_error(std::string("Listen failed: ") + strerror(errno));
    }
}

void LoadBalancerServer::LoadBalancerProcess() { ServerLoop(); }

SocketWrapper&& LoadBalancerServer::AcceptClient() const {
    struct sockaddr_storage client_addr {};
    socklen_t client_len = sizeof(client_addr);

    int client_fd =
        accept(server_socket_.get()->GetSocketFileDescriptor(), (struct sockaddr*)&client_addr, &client_len);
    if (client_fd == -1) {
        throw std::runtime_error(std::string("Accept failed: ") + strerror(errno));
    }

    SocketWrapper client_socket(client_fd);
    return st::move(client_socket);
}

char* LoadBalancerServer::RecieveRequestFromClient(SocketWrapper&& client_socket_wrapper) const{
    char request_buffer[1024];
    ssize_t bytes_received =
        recv(client_socket_wrapper.GetSocketFileDescriptor(), request_buffer, sizeof(request_buffer), 0);
    if (bytes_received <= 0) {
        throw std::runtime_error("Failed to receive data");
    } 
}

SocketWrapper LoadBalancerServer::EstablishConnectionWithRemoteServer();
void LoadBalancerServer::SendRequestToRemoteServer() const;
std::string LoadBalancerServer::ReceiveResponseFromRemoteServer() const;
void LoadBalancerServer::SendResponseToClient() const;

void LoadBalancerServer::ServerLoop() {
    while (true) {
        try {
            // Accept a client

            // RETURN: SocketWrapper

            // Recieve a request from a client

            // RETURN: request_buffer

            // Resend request to a remote servers
            if (!servers_.empty()) {
                // Establish connection with the remote server
                struct addrinfo hints;
                memset(&hints, 0, sizeof(hints));
                hints.ai_socktype = SOCK_STREAM;
                struct addrinfo* remote_server;
                getaddrinfo(servers_[0].first, servers_[0].second, &hints, &remote_server);

                SocketWrapper remote_server_socket(
                    socket(remote_server->ai_family, remote_server->ai_socktype, remote_server->ai_protocol));
                if (connect(remote_server_socket.GetSocketFileDescriptor(), remote_server->ai_addr,
                            remote_server->ai_addrlen) == -1) {
                    std::cout << "Can not connect to remote server!\n";
                }

                freeaddrinfo(remote_server);

                // RETURN: SocketWrapper

                // Resend request to the remote server
                send(remote_server_socket.GetSocketFileDescriptor(), request_buffer, sizeof(request_buffer), 0);
                // RETURN: void

                // Receive response from a remote server
                char receive_buffer[1024] = {0};
                std::string full_response;
                ssize_t bytes_received = 0;

                while ((bytes_received = recv(remote_server_socket.GetSocketFileDescriptor(), receive_buffer,
                                              sizeof(receive_buffer) - 1, 0)) > 0) {
                    receive_buffer[bytes_received] = '\0';
                    full_response += receive_buffer;
                }

                std::cout << "Will send " << full_response << "...\n";

                // RETURN: full_response

                // Send response back to client
                if (full_response.size() > 0) {
                    ssize_t bytes_sent = send(client_socket.GetSocketFileDescriptor(), full_response.c_str(),
                                              strlen(full_response.c_str()), 0);
                    if (bytes_sent == -1) {
                        std::cout << "Failed to send response to the client\n";
                    }
                    std::cout << "Sent " << bytes_sent << "\n";
                }
                // RETURN: void
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void LoadBalancerServer::DEBUG_PushTestServer() {
    servers_.push_back(std::pair<const char*, const char*>("127.0.0.1", "8080"));
}