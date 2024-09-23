#include "load-balancer-roundrobin.hpp"

void LoadBalancerServerRoundRobin::DEBUG_PushServers() { DEBUG_PushFiveTestServers(); };

void LoadBalancerServerRoundRobin::LoadBalancing() {
    while (true) {
        try {
            for (size_t i = 0; i < servers_.size(); i++) {
                client_com_handler_.AcceptClient(load_balancer_socket_wrapper_);

                std::string client_requst = client_com_handler_.RecieveRequestFromClient();

                server_com_handler_.EstablishConnectionWithRemoteServer(servers_[i]);

                server_com_handler_.SendRequestToRemoteServer(servers_[i], client_requst);

                std::string server_response = server_com_handler_.ReceiveResponseFromRemoteServer(servers_[i]);

                client_com_handler_.SendResponseToClient(server_response);

                client_com_handler_.CloseClientSocket();
            }
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}