#include <signal.h>

#include "load-balancer-pseudo.hpp"
#include "load-balancer-roundrobin.hpp"

int main() {
    signal(SIGPIPE, SIG_IGN);  // Ignore SIGPIPE

    try {
        std::unique_ptr<LoadBalancerServerInterface> load_balancer;
        // TODO: after JSON parser will be created, switch shall take decision based on the user input in a json file
        // TODO: after JSON parser will be implemented remove this dummy variable
        int lb_type = 1;  // TODO ASAP: replace this var with the var from a command line
        switch (lb_type) {
            case 0:
                load_balancer = std::make_unique<LoadBalancerServerPseudo>();
                break;
            case 1:
                load_balancer = std::make_unique<LoadBalancerServerRoundRobin>();
                break;
            default:
                break;
        }

        load_balancer.get()->DEBUG_PushServers();
        load_balancer.get()->StartLoadBalancerServer();
        load_balancer.get()->LoadBalancing();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
