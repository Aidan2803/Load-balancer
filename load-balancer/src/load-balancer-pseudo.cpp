#include "balancer/load-balancer-pseudo.hpp"

LoadBalancerServerPseudo::LoadBalancerServerPseudo(const std::string &instance_name, const std::string &ip,
                                                   const std::string &port)
    : LoadBalancerServerInterface(instance_name, ip, port) {}

void LoadBalancerServerPseudo::HandleClient(ServerComHandler &server_com_handler,
                                            std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper,
                                            std::vector<ServerInfo> &servers, int server_number) {
    ClientComHandler client_handler;

    client_handler.AcceptClient(load_balancer_socket_wrapper);

    std::string client_requst = client_handler.RecieveRequestFromClient();

    spdlog::info("{} Received client request: {}", instance_name_, client_requst);

    server_com_handler_.EstablishConnectionWithRemoteServer(servers[server_number]);

    server_com_handler.SendRequestToRemoteServer(servers[server_number], client_requst);

    spdlog::info("{} Sent request to remote server", instance_name_);

    std::string server_response = server_com_handler.ReceiveResponseFromRemoteServer(servers[server_number]);

    client_handler.SendResponseToClient(server_response);

    client_handler.CloseClientSocket();
}

void LoadBalancerServerPseudo::LoadBalancing() {
    struct pollfd polling_fd;
    polling_fd.fd = load_balancer_socket_wrapper_->GetSocketFileDescriptor();
    polling_fd.events = POLLIN;

    int max_concurrent_tasks = thread_pool_->GetMaxThreadsAmount();

    while (true) {
        try {
            int poll_ret = poll(&polling_fd, 1, -1);
            if (poll_ret > 0 && (polling_fd.revents == POLLIN)) {
                if (thread_pool_->GetCurrentTasksAmount() < max_concurrent_tasks) {
                    thread_pool_->EnqueueTask(
                        // 0 is the first and only server in the servers vector
                        [this]() { HandleClient(server_com_handler_, load_balancer_socket_wrapper_, servers_, 0); });
                }
            } else if (polling_fd.revents & POLLERR) {
                int err = 0;
                socklen_t error_len = sizeof(err);
                getsockopt(load_balancer_socket_wrapper_->GetSocketFileDescriptor(), SOL_SOCKET, SO_ERROR, &err,
                           &error_len);
                spdlog::error("{}  Socket error: {}", instance_name_, strerror(err));
            }

        } catch (const std::exception &e) {
            spdlog::error("{} {}", instance_name_, e.what());
        }
    }
}

void LoadBalancerServerPseudo::DEBUG_PushServers() { DEBUG_PushTestServer(); };
