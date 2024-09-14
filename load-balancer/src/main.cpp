#include "load-balancer.hpp"

int main(int argc, char** argv) {
    try {
        LoadBalancerServer lb{};
        lb.LoadBalancerProcess();        
    } catch (const std::exception& e) {
        std::cerr << "Fatal error: " << e.what() << std::endl;
        return EXIT_FAILURE;
    }
}
