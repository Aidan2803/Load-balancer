#ifndef SERVER_COM_HANDLER_HPP
#define SERVER_COM_HANDLER_HPP

#include <iostream>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

#include "SocketWrapper.hpp"
#include "server-info.hpp"

class ServerComHandler {
  public:
    ServerComHandler();
    ~ServerComHandler();
    void EstablishConnectionWithRemoteServer(ServerInfo &server);
    void SendRequestToRemoteServer(ServerInfo &server, std::string &request_buffer);
    std::string ReceiveResponseFromRemoteServer(ServerInfo &server);

  private:
    std::string GetIpPortKeyFormat(const std::string &ip, const std::string &port) const;
    std::optional<std::unordered_map<std::string, std::unique_ptr<SocketWrapper>>::const_iterator> FindSocketByIpPort(
        const std::string &ip, const std::string &port);
    std::optional<std::unordered_map<std::string, std::unique_ptr<SocketWrapper>>::const_iterator> FindSocketByIpPort(
        std::string &ipport_key);

  private:
    // std::mutex server_com_handler_mutex_;
    std::unordered_map<std::string, std::unique_ptr<SocketWrapper>> server_ipport_to_socket_map_;
};

#endif