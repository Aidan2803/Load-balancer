#include "load-balancer-pseudo.hpp"

void LoadBalancerServerPseudo::LoadBalancing() {
    while (true) {
        try {
            client_com_handler_.AcceptClient(load_balancer_socket_wrapper_);

            std::string client_requst = client_com_handler_.RecieveRequestFromClient();

            std::cout << "[LoadBalancerServerPseudo] Received client request: " << client_requst << std::endl;
            
            server_com_handler_.EstablishConnectionWithRemoteServer(servers_[0]);

            server_com_handler_.SendRequestToRemoteServer(servers_[0], client_requst);

            std::cout << "[LoadBalancerServerPseudo] Sent request to remote server" << std::endl;

            std::string server_response = server_com_handler_.ReceiveResponseFromRemoteServer(servers_[0]);

            client_com_handler_.SendResponseToClient(server_response);

            client_com_handler_.CloseClientSocket();

            std::cout << "[LoadBalancerServerPseudo] loop\n\n\n";

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}