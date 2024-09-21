#include "client-com-handler.hpp"

ClientComHandler::ClientComHandler() {}

ClientComHandler::~ClientComHandler() {}

void ClientComHandler::AcceptClient(std::unique_ptr<SocketWrapper> &load_balancer_socket_wrapper) {
    struct sockaddr_storage client_addr {};
    socklen_t client_len = sizeof(client_addr);

    int client_fd = accept(load_balancer_socket_wrapper.get()->GetSocketFileDescriptor(),
                           (struct sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        throw std::runtime_error(std::string("[ClientComHandler] Accept failed: ") + strerror(errno));
    }

    std::cout << "[ClientComHandler] Client socket: " << client_fd << "\n";
    client_socket_wrapper_ = std::make_unique<SocketWrapper>(client_fd);
}

std::string ClientComHandler::RecieveRequestFromClient() {
    char request_buffer[1024];
    ssize_t bytes_received =
        recv(client_socket_wrapper_.get()->GetSocketFileDescriptor(), request_buffer, sizeof(request_buffer), 0);
    if (bytes_received <= 0) {
        throw std::runtime_error("[ClientComHandler] Failed to receive data");
    }

    return request_buffer;
}

void ClientComHandler::SendResponseToClient(std::string &full_response) {
    if (full_response.size() > 0) {
        ssize_t bytes_sent = send(client_socket_wrapper_.get()->GetSocketFileDescriptor(), full_response.c_str(),
                                  strlen(full_response.c_str()), 0);
        if (bytes_sent == -1) {
            std::cout << "[ClientComHandler] Failed to send response to the client\n";
        }
        std::cout << "[ClientComHandler] Sent " << bytes_sent << " bytes to the client\n";
    }
}

void ClientComHandler::CloseClientSocket() {
    std::cout << "[ClientComHandler] Closing client socket\n";
    client_socket_wrapper_.reset();
}