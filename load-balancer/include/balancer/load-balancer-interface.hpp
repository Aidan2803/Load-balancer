#ifndef LOAD_BALANCER_INTERFACE
#define LOAD_BALANCER_INTERFACE

#include <exception>
#include <iostream>
#include <memory>
#include <mutex>
#include <thread>
#include <vector>

#include "SocketWrapper.hpp"
#include "balancer/client-com-handler.hpp"
#include "parser/iparser.hpp"
#include "balancer/server-com-handler.hpp"
#include "balancer/server-info.hpp"
#include "spdlog/spdlog.h"
#include "utility/thread-pool.hpp"

class LoadBalancerServerInterface {
  public:
    LoadBalancerServerInterface(const std::string &instance_name = "[LoadBalancerServerInterface]");

    virtual void LoadBalancing() = 0;

    void SetServersInfo(std::vector<ServerInfo> servers) { servers_ = servers; }
    void SetIsKeepAliveInfo(bool is_keep_alive) { is_keep_alive_connection_ = is_keep_alive; }
    void SetBacklogSize(int backlog_size) { backlog_size_ = backlog_size; }

    virtual void HandleClient(ServerComHandler &server_com_handler,
                              std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper,
                              std::vector<ServerInfo> &servers, int server_number) = 0;
    virtual void DEBUG_PushServers() = 0;

    void StartLoadBalancerServer();

  protected:
    void DEBUG_PushTestServer();
    void DEBUG_PushFiveTestServers();

  protected:
    std::vector<ServerInfo> servers_;
    bool is_keep_alive_connection_;

    const char *port_{"8090"};  // TODO: add this field to json parsing
    int backlog_size_;

    std::mutex load_balancer_mutex_;
    std::unique_ptr<ThreadPool> thread_pool_;

    std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper_;
    ServerComHandler server_com_handler_;
    const std::string instance_name_;
};

#endif