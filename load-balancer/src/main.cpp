#include "load-balancer-pseudo.hpp"
// #include "load-balancer-roundrobin.hpp"

int main() {
    try {
        std::unique_ptr<LoadBalancerServerInterface> load_balancer;
        // TODO: after JSON parser will be created, switch shall take decision based on the user input in a json file
        // TODO: after JSON parser will be implemented remove this dummy variable
        int lb_type = 0;
        switch (lb_type) {
            case 0:
                load_balancer = std::make_unique<LoadBalancerServerPseudo>();
                break;
            // case 1:
            //     load_balancer = std::make_unique<LoadBalancerServerRoundRobin>();
            //     break;
            default:
                break;
        }

        load_balancer.get()->LoadBalancing();
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
