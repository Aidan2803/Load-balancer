#include "load-balancer-pseudo.hpp"

LoadBalancerServerPseudo::LoadBalancerServerPseudo(const std::string &instance_name)
    : LoadBalancerServerInterface(instance_name) {}

void LoadBalancerServerPseudo::HandleClient(ServerComHandler &server_com_handler,
                                            std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper,
                                            ServerInfo &server) {
    std::lock_guard<std::mutex> load_balancer_lock_guard(load_balancer_mutex_);

    ClientComHandler client_handler;

    client_handler.AcceptClient(load_balancer_socket_wrapper);

    std::string client_requst = client_handler.RecieveRequestFromClient();

    spdlog::info("{} Received client request: {}", instance_name_, client_requst);

    server_com_handler.EstablishConnectionWithRemoteServer(server);

    server_com_handler.SendRequestToRemoteServer(servers_[0], client_requst);

    spdlog::info("{} Sent request to remote server", instance_name_);

    std::string server_response = server_com_handler.ReceiveResponseFromRemoteServer(server);

    client_handler.SendResponseToClient(server_response);

    client_handler.CloseClientSocket();
}

void LoadBalancerServerPseudo::LoadBalancing() {
    struct pollfd fds[1];
    fds[0].fd = load_balancer_socket_wrapper_->GetSocketFileDescriptor();
    fds[0].events = POLLIN;

    while (true) {
        try {
            int poll_ret = poll(fds, 1, -1);
            if (poll_ret > 0 && (fds[0].revents == POLLIN)) {
                thread_pool_->EnqueueTask(
                    [this]() { HandleClient(server_com_handler_, load_balancer_socket_wrapper_, servers_[0]); });
            }

        } catch (const std::exception &e) {
            spdlog::error("{} {}", instance_name_, e.what());
        }
    }
}

void LoadBalancerServerPseudo::DEBUG_PushServers() { DEBUG_PushTestServer(); };
