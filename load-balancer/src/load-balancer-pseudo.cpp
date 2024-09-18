#include "load-balancer-pseudo.hpp"

void LoadBalancerServerPseudo::LoadBalancing() {
    while (true) {
        try {
            client_com_handler_.AcceptClient(load_balancer_socket_wrapper_);

            std::string client_requst = client_com_handler_.RecieveRequestFromClient();

            server_com_handler_.SendRequestToRemoteServer(servers_[0], client_requst);

            std::string server_response = server_com_handler_.ReceiveResponseFromRemoteServer(servers_[0]);

            client_com_handler_.SendResponseToClient(server_response);

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    }
}