#include "server-com-handler.hpp"

ServerComHandler::ServerComHandler() : instance_name_{"[ServerComHandler]"} {}

std::string ServerComHandler::GetIpPortKeyFormat(const std::string &ip, const std::string &port) const {
    return ip + ':' + port;
}

std::optional<std::unordered_map<std::string, std::unique_ptr<SocketWrapper>>::const_iterator>
ServerComHandler::FindSocketByIpPort(const std::string &ip, const std::string &port) {
    std::string ipport = GetIpPortKeyFormat(ip, port);
    auto iterator = server_ipport_to_socket_map_.find(ipport);
    if (iterator != server_ipport_to_socket_map_.end()) {
        return iterator;
    } else {
        spdlog::warn("{} Socket for {} does not extist!", instance_name_, ipport);
        return std::nullopt;
    }
}

std::optional<std::unordered_map<std::string, std::unique_ptr<SocketWrapper>>::const_iterator>
ServerComHandler::FindSocketByIpPort(std::string &ipport_key) {
    auto iterator = server_ipport_to_socket_map_.find(ipport_key);
    if (iterator != server_ipport_to_socket_map_.end()) {
        return iterator;
    } else {
        spdlog::warn("{} Socket for {} does not extist!", instance_name_, ipport_key);
        return std::nullopt;
    }
}

void ServerComHandler::EstablishConnectionWithRemoteServer(ServerInfo &server) {
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo *remote_server;

    getaddrinfo(server.ip_.c_str(), server.port_.c_str(), &hints, &remote_server);

    std::string ipport = GetIpPortKeyFormat(server.ip_, server.port_);

    server_com_handler_mutex_.lock();
    server_ipport_to_socket_map_[ipport] = std::make_unique<SocketWrapper>(
        remote_server->ai_family, remote_server->ai_socktype, remote_server->ai_protocol);
    server_com_handler_mutex_.unlock();

    server.is_available_ = true;

    auto iterator = FindSocketByIpPort(ipport);
    if (iterator) {
        server_com_handler_mutex_.lock();
        if (connect(iterator.value()->second->GetSocketFileDescriptor(), remote_server->ai_addr,
                    remote_server->ai_addrlen) == -1) {
            spdlog::info("{} Can not connect to remote server!", instance_name_);
            server.is_available_ = false;
        }
        server_com_handler_mutex_.unlock();
    }

    for (const auto &pair : server_ipport_to_socket_map_) {
        spdlog::debug("IpPort: {}, Socket FD {}", pair.first, pair.second->GetSocketFileDescriptor());
    }

    freeaddrinfo(remote_server);
}

void ServerComHandler::SendRequestToRemoteServer(ServerInfo &server, std::string &request_buffer) {
    auto iterator = FindSocketByIpPort(server.ip_, server.port_);
    if (iterator) {
        server_com_handler_mutex_.lock();
        auto sent_size = send(iterator.value()->second->GetSocketFileDescriptor(), request_buffer.c_str(),
                              sizeof(request_buffer.c_str()), 0);
        server_com_handler_mutex_.unlock();
        if (sent_size == -1) {
            if (errno == EPIPE) {
                spdlog::critical("{} Send failed, socket closed! {}", instance_name_,
                                 std::to_string(iterator.value()->second->GetSocketFileDescriptor()));
                throw(std::runtime_error("[ServerComHandler] Send failed, socket closed! " +
                                         std::to_string(iterator.value()->second->GetSocketFileDescriptor()) + "\n"));
            }
        }
    }
}

std::string ServerComHandler::ReceiveResponseFromRemoteServer(ServerInfo &server) {
    char receive_buffer[1024] = {0};
    std::string full_response;
    ssize_t bytes_received = 0;

    auto iterator = FindSocketByIpPort(server.ip_, server.port_);
    if (iterator) {
        server_com_handler_mutex_.lock();
        while ((bytes_received = recv(iterator.value()->second->GetSocketFileDescriptor(), receive_buffer,
                                      sizeof(receive_buffer) - 1, 0)) > 0) {
            receive_buffer[bytes_received] = '\0';
            full_response += receive_buffer;
        }
        server_com_handler_mutex_.unlock();
    }

    if (!full_response.empty()) {
        spdlog::info(
            "{} {} {} Will send a response message: \n ---Message starts here---\n {} ---Message ends here---\n",
            instance_name_, server.ip_, server.port_, full_response);
    } else {
        spdlog::error("{} Received an empty response from the remote server! {}:{}", instance_name_, server.ip_,
                      server.port_);
    }

    return full_response;
}