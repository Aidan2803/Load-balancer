#include "server-com-handler.hpp"

ServerComHandler::ServerComHandler() {}

ServerComHandler::~ServerComHandler() {}

std::string ServerComHandler::GetIpPortKeyFormat(const std::string &ip, const std::string &port) const {
    return ip + ':' + port;
}

std::optional<std::map<std::string, std::unique_ptr<SocketWrapper>>::const_iterator>
ServerComHandler::FindSocketByIpPort(const std::string &ip, const std::string &port) {
    std::string ipport = GetIpPortKeyFormat(ip, port);
    auto iterator = server_ipport_to_socket_map_.find(ipport);
    if (iterator != server_ipport_to_socket_map_.end()) {
        return iterator;
    } else {
        std::cout << "[ServerComHandler] Socket for " << ipport << " does not extist!\n";
        return std::nullopt;
    }
}

std::optional<std::map<std::string, std::unique_ptr<SocketWrapper>>::const_iterator>
ServerComHandler::FindSocketByIpPort(std::string &ipport_key) {
    auto iterator = server_ipport_to_socket_map_.find(ipport_key);
    if (iterator != server_ipport_to_socket_map_.end()) {
        return iterator;
    } else {
        std::cout << "[ServerComHandler] Socket for " << ipport_key << " does not extist!\n";
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

    server_ipport_to_socket_map_[ipport] = std::make_unique<SocketWrapper>(
        remote_server->ai_family, remote_server->ai_socktype, remote_server->ai_protocol);

    server.is_available_ = true;

    auto iterator = FindSocketByIpPort(ipport);
    if (iterator) {
        if (connect(iterator.value()->second.get()->GetSocketFileDescriptor(), remote_server->ai_addr,
                    remote_server->ai_addrlen) == -1) {
            std::cout << "[ServerComHandler] Can not connect to remote server!\n";
            server.is_available_ = false;
        }
    }

    freeaddrinfo(remote_server);
}

void ServerComHandler::SendRequestToRemoteServer(ServerInfo &server, std::string &request_buffer) {
    auto iterator = FindSocketByIpPort(server.ip_, server.port_);
    if (iterator) {
        send(iterator.value()->second.get()->GetSocketFileDescriptor(), request_buffer.c_str(),
             sizeof(request_buffer.c_str()), 0);
    }
}

std::string ServerComHandler::ReceiveResponseFromRemoteServer(ServerInfo &server) {
    char receive_buffer[1024] = {0};
    std::string full_response;
    ssize_t bytes_received = 0;

    auto iterator = FindSocketByIpPort(server.ip_, server.port_);
    if (iterator) {
        while ((bytes_received = recv(iterator.value()->second.get()->GetSocketFileDescriptor(), receive_buffer,
                                      sizeof(receive_buffer) - 1, 0)) > 0) {
            receive_buffer[bytes_received] = '\0';
            full_response += receive_buffer;
        }
    }

    std::cout << "[ServerComHandler] " << server.ip_ << ":" << server.port_ << " Will send message: \n"
              << "---Message starts here---\n"
              << full_response << "---Message ends here---\n";

    return full_response;
}