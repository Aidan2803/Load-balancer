#include "balancer/load-balancer-roundrobin.hpp"

LoadBalancerServerRoundRobin::LoadBalancerServerRoundRobin(const std::string &instance_name, const std::string &ip,
                                                           const std::string &port)
    : LoadBalancerServerInterface(instance_name, ip, port) {
    spdlog::info("{} Starting load-balancer at {}:{}", instance_name_, ip, port);
}

void LoadBalancerServerRoundRobin::HandleClient(ServerComHandler &server_com_handler,
                                                std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper,
                                                std::vector<ServerInfo> &servers, int server_number) {
    spdlog::info("{} Handle a client by the {} {} ", instance_name_,
                 std::hash<std::thread::id>{}(std::this_thread::get_id()), servers[server_number].port_);

    ClientComHandler client_handler;

    client_handler.AcceptClient(load_balancer_socket_wrapper_);

    std::string client_requst = client_handler.RecieveRequestFromClient();

    bool at_least_one_server_avalable = true;
    server_com_handler.EstablishConnectionWithRemoteServer(servers[server_number]);

    if (!servers[server_number].is_available_) {
        spdlog::debug("{} server not available", instance_name_);
        int unavailable_servers_amount = 1;

        while (!servers[server_number].is_available_) {
            spdlog::debug("{} go into establish one more time", instance_name_);
            server_number = (server_number + 1) % servers_.size();
            server_com_handler.EstablishConnectionWithRemoteServer(servers[server_number]);

            if (!servers[server_number].is_available_) {
                unavailable_servers_amount++;
            }

            if (unavailable_servers_amount == servers_.size()) {
                spdlog::error("{} No available servers to handle request!", instance_name_);
                at_least_one_server_avalable = false;
                break;
            }
        }
        spdlog::debug("{} after loop", instance_name_);
    }
    if (at_least_one_server_avalable) {
        server_com_handler.SendRequestToRemoteServer(servers[server_number], client_requst);

        std::string server_response = server_com_handler_.ReceiveResponseFromRemoteServer(servers[server_number]);

        client_handler.SendResponseToClient(server_response);

        client_handler.CloseClientSocket();
    } else {
        // TODO: send client an error code since connection with the client is already established
    }
}

void LoadBalancerServerRoundRobin::LoadBalancing() {
    struct pollfd polling_fd;
    polling_fd.fd = load_balancer_socket_wrapper_->GetSocketFileDescriptor();
    polling_fd.events = POLLIN;

    int server_number_iterator = 0;
    const int max_concurrent_tasks = thread_pool_->GetMaxThreadsAmount();
    bool added_one_task = false;

    while (true) {
        try {
            int poll_ret = poll(&polling_fd, 1, -1);

            if (poll_ret > 0 && (polling_fd.revents & POLLIN) && !added_one_task) {
                if (thread_pool_->GetCurrentTasksAmount() < max_concurrent_tasks) {
                    added_one_task = true;

                    thread_pool_->EnqueueTask([this, &server_number_iterator, &added_one_task]() {
                        HandleClient(server_com_handler_, load_balancer_socket_wrapper_, servers_,
                                     server_number_iterator);
                        spdlog::info("{} Current amount of tasks: {}", instance_name_,
                                     thread_pool_->GetCurrentTasksAmount());

                        server_number_iterator = (server_number_iterator + 1) % servers_.size();
                        spdlog::debug("{} Server number iterator {}", instance_name_, server_number_iterator);

                        added_one_task = false;
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
