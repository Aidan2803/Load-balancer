#include "client-com-handler.hpp"

ClientComHandler::ClientComHandler() : instance_name_{"[ClientHandler]"} {}

void ClientComHandler::AcceptClient(std::shared_ptr<SocketWrapper> &load_balancer_socket_wrapper) {
    sockaddr_storage client_addr{};
    socklen_t client_len = sizeof(client_addr);

    int client_fd =
        accept(load_balancer_socket_wrapper->GetSocketFileDescriptor(), (sockaddr *)&client_addr, &client_len);
    if (client_fd == -1) {
        spdlog::critical("{} Accept failed: {}", instance_name_, strerror(errno));
        throw std::runtime_error(std::string("[ClientComHandler] Accept failed: ") + strerror(errno));
    }

    spdlog::info("{} Client socket: {}", instance_name_, client_fd);
    client_socket_wrapper_ = std::make_unique<SocketWrapper>(client_fd);
}

std::string ClientComHandler::RecieveRequestFromClient() {
    char request_buffer[1024];
    ssize_t bytes_received =
        recv(client_socket_wrapper_->GetSocketFileDescriptor(), request_buffer, sizeof(request_buffer), 0);
    if (bytes_received <= 0) {
        spdlog::critical("{} Failed to receive data: {}", instance_name_, strerror(errno));
        throw std::runtime_error(instance_name_ + " Failed to receive data");
    }

    return request_buffer;
}

void ClientComHandler::SendResponseToClient(std::string &full_response) {
    if (full_response.size() > 0) {
        ssize_t bytes_sent =
            send(client_socket_wrapper_->GetSocketFileDescriptor(), full_response.c_str(), full_response.size(), 0);
        if (bytes_sent == -1) {
            spdlog::error("{} Failed to send response to the client: {}", instance_name_, strerror(errno));
        }
        spdlog::info("{} Sent {} bytes to the client", instance_name_, bytes_sent);
    }
}

void ClientComHandler::CloseClientSocket() {
    spdlog::info("{} Closing client socket", instance_name_);
    client_socket_wrapper_.reset();
}