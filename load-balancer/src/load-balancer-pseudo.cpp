#include "load-balancer-pseudo.hpp"

#include "thread-pool.hpp"

void LoadBalancerServerPseudo::HandleClient(ServerComHandler &server_com_handler, ClientComHandler &client_com_handler,
                                            std::shared_ptr<SocketWrapper> load_balancer_socket_wrapper,
                                            ServerInfo &server) {
    std::lock_guard<std::mutex> load_balancer_lock_guard(load_balancer_mutex_);

    client_com_handler_.AcceptClient(load_balancer_socket_wrapper);

    std::string client_requst = client_com_handler_.RecieveRequestFromClient();

    std::cout << "[LoadBalancerServerPseudo] Received client request: " << client_requst << std::endl;

    server_com_handler.EstablishConnectionWithRemoteServer(server);

    server_com_handler.SendRequestToRemoteServer(servers_[0], client_requst);

    std::cout << "[LoadBalancerServerPseudo] Sent request to remote server" << std::endl;

    std::string server_response = server_com_handler.ReceiveResponseFromRemoteServer(server);

    client_com_handler.SendResponseToClient(server_response);

    client_com_handler.CloseClientSocket();
}

void LoadBalancerServerPseudo::LoadBalancing() {
    while (true) {
        try {
            ThreadPool tp(1);

            tp.EnqueueTask([this]() {
                HandleClient(server_com_handler_, client_com_handler_, load_balancer_socket_wrapper_, servers_[0]);
            });

        } catch (const std::exception &e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}

void LoadBalancerServerPseudo::DEBUG_PushServers() { DEBUG_PushTestServer(); };
