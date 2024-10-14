#include "load-balancer-roundrobin.hpp"

LoadBalancerServerRoundRobin::LoadBalancerServerRoundRobin(const std::string &instance_name)
    : LoadBalancerServerInterface(instance_name) {}

void LoadBalancerServerRoundRobin::HandleClient(ServerComHandler &server_com_handler,
                                                std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper,
                                                ServerInfo &server) {
    spdlog::info("{} Handle a client by the {}", instance_name_,
                 std::hash<std::thread::id>{}(std::this_thread::get_id()));

    ClientComHandler client_handler;

    client_handler.AcceptClient(load_balancer_socket_wrapper_);

    std::string client_requst = client_handler.RecieveRequestFromClient();

    server_com_handler.EstablishConnectionWithRemoteServer(server);

    server_com_handler.SendRequestToRemoteServer(server, client_requst);

    std::string server_response = server_com_handler_.ReceiveResponseFromRemoteServer(server);

    client_handler.SendResponseToClient(server_response);

    client_handler.CloseClientSocket();
}

void LoadBalancerServerRoundRobin::LoadBalancing() {
    struct pollfd polling_fd;
    polling_fd.fd = load_balancer_socket_wrapper_->GetSocketFileDescriptor();
    polling_fd.events = POLLIN;

    int server_number_iterator = 0;
    const int max_concurrent_tasks = thread_pool_->GetMaxThreadsAmount();
    while (true) {
        try {
            int poll_ret = poll(&polling_fd, 1, -1);

            if (poll_ret > 0 && (polling_fd.revents & POLLIN)) {
                if (thread_pool_->GetCurrentTasksAmount() < max_concurrent_tasks) {
                    thread_pool_->EnqueueTask([this, &server_number_iterator]() {
                        HandleClient(server_com_handler_, load_balancer_socket_wrapper_,
                                     servers_[server_number_iterator]);
                        spdlog::info("{} Current amount of tasks: {}", instance_name_,
                                     thread_pool_->GetCurrentTasksAmount());

                        server_number_iterator = (server_number_iterator + 1) % servers_.size();
                    });
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

void LoadBalancerServerRoundRobin::DEBUG_PushServers() { DEBUG_PushFiveTestServers(); };
