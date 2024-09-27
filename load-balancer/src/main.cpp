#include <signal.h>

#include "load-balancer-pseudo.hpp"
#include "load-balancer-roundrobin.hpp"

int main(int agrc, char** argv) {
    signal(SIGPIPE, SIG_IGN);  // Ignore SIGPIPE

    try {
        std::unique_ptr<LoadBalancerServerInterface> load_balancer;
        // TODO: after JSON parser will be created, switch shall take decision based on the user input in a json file
        // TODO: after JSON parser will be implemented remove this command line arguments pars
        int lb_type = 1;  // TODO ASAP: replace this var with the var from a command line

        if (agrc == 2) {
            switch (argv[1][0]) {
                case '0':
                    load_balancer = std::make_unique<LoadBalancerServerPseudo>();
                    break;
                case '1':
                    load_balancer = std::make_unique<LoadBalancerServerRoundRobin>();
                    break;
                default:
                    break;
            }
        } else {
            load_balancer = std::make_unique<LoadBalancerServerPseudo>();
        }

        load_balancer->DEBUG_PushServers();
        load_balancer->StartLoadBalancerServer();
        load_balancer->LoadBalancing();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
