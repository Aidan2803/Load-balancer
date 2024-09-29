#include "load-balancer-roundrobin.hpp"

void LoadBalancerServerRoundRobin::HandleClient(ServerComHandler &server_com_handler,
                                                ClientComHandler &client_com_handler,
                                                std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper,
                                                ServerInfo &server) {
    std::lock_guard<std::mutex> load_balancer_lock_guard(load_balancer_mutex_);
    std::cout << "Handle a client\n";

    client_com_handler.AcceptClient(load_balancer_socket_wrapper_);

    std::string client_requst = client_com_handler_.RecieveRequestFromClient();

    server_com_handler.EstablishConnectionWithRemoteServer(server);

    server_com_handler.SendRequestToRemoteServer(server, client_requst);

    std::string server_response = server_com_handler_.ReceiveResponseFromRemoteServer(server);

    client_com_handler.SendResponseToClient(server_response);

    client_com_handler.CloseClientSocket();
}

void LoadBalancerServerRoundRobin::LoadBalancing() {
    struct pollfd polling_fd;
    polling_fd.fd = load_balancer_socket_wrapper_->GetSocketFileDescriptor();
    polling_fd.events = POLLIN;

    int server_number_iterator = 0;
    while (true) {
        try {
            int poll_ret = poll(&polling_fd, 1, -1);

            if (poll_ret > 0 && (polling_fd.revents & POLLIN)) {
                if (!task_for_thread_created_.exchange(true)) {
                    thread_pool_->EnqueueTask([this, &server_number_iterator]() {
                        HandleClient(server_com_handler_, client_com_handler_, load_balancer_socket_wrapper_,
                                     servers_[server_number_iterator]);

                        server_number_iterator = (server_number_iterator + 1) % servers_.size();
                        task_for_thread_created_ = false;
                    });
                }

            } else if (polling_fd.revents & POLLERR) {
                int err = 0;
                socklen_t error_len = sizeof(err);
                getsockopt(load_balancer_socket_wrapper_->GetSocketFileDescriptor(), SOL_SOCKET, SO_ERROR, &err,
                           &error_len);
                std::cerr << "[LoadBalancerServerRoundRobin] Socket error: " << strerror(err) << std::endl;
            }

        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void LoadBalancerServerRoundRobin::DEBUG_PushServers() { DEBUG_PushFiveTestServers(); };
