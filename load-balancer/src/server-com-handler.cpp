#include "server-com-handler.hpp"

ServerComHandler::ServerComHandler(){}

ServerComHandler::~ServerComHandler(){}

void ServerComHandler::EstablishConnectionWithRemoteServer(std::pair<const char*, const char*> server){
    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    struct addrinfo* remote_server;

    getaddrinfo(server.first, server.second, &hints, &remote_server);

    server_socket_wrapper_ = std::make_unique<SocketWrapper>(remote_server->ai_family, remote_server->ai_socktype, remote_server->ai_protocol);

    if (connect(server_socket_wrapper_.get()->GetSocketFileDescriptor(), remote_server->ai_addr,
                remote_server->ai_addrlen) == -1) {
        std::cout << "[ServerComHandler] Can not connect to remote server!\n";
    }

    freeaddrinfo(remote_server);
}

void ServerComHandler::SendRequestToRemoteServer(std::string &request_buffer) const {
    send(server_socket_wrapper_.get()->GetSocketFileDescriptor(), request_buffer.c_str(), sizeof(request_buffer.c_str()), 0);
}

std::string ServerComHandler::ReceiveResponseFromRemoteServer() const {
    char receive_buffer[1024] = {0};
    std::string full_response;
    ssize_t bytes_received = 0;

    while ((bytes_received = recv(server_socket_wrapper_.get()->GetSocketFileDescriptor(), receive_buffer,
                                  sizeof(receive_buffer) - 1, 0)) > 0) {
        receive_buffer[bytes_received] = '\0';
        full_response += receive_buffer;
    }

    std::cout << "[ServerComHandler] Will send " << full_response << "...\n";

    return full_response;

}