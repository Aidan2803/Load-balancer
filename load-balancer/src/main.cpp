#include <signal.h>

#include "load-balancer-pseudo.hpp"
#include "load-balancer-roundrobin.hpp"
#include "settings-setter.hpp"
#include "parser-types.hpp"
#include "spdlog/spdlog.h"

ParserTypes AnalyzeFileType(char *file_name){
    std::string file_name_str(file_name);

    std::size_t file_ext_index = file_name_str.rfind(".json");
    if(file_ext_index == file_name_str.size() - 5){
        ParserTypes parser_type = ParserTypes::JSONParser;
        return parser_type;
    }

    file_ext_index = file_name_str.rfind(".txt");
    if(file_ext_index == file_name_str.size() - 4){
        ParserTypes parser_type = ParserTypes::TXTParser;
        return parser_type;
    } else {
        throw std::runtime_error("Unknown file extension " + file_name_str);
    }
}

int main(int agrc, char** argv) {
    signal(SIGPIPE, SIG_IGN);  // Ignore SIGPIPE
    spdlog::set_level(spdlog::level::debug);

    const std::string instance_name{"[Main]"};

    try {
        ParserTypes parser_type = AnalyzeFileType(argv[1]);

        std::shared_ptr<LoadBalancerServerInterface> load_balancer;
        SettingsSetter settings_setter(load_balancer, parser_type);
        // TODO: after JSON parser will be created, switch shall take decision based on the user input in a json file
        // TODO: after JSON parser will be implemented remove this command line arguments pars

        if (agrc == 2) {
            switch (argv[1][0]) {
                case '0':
                    load_balancer = std::make_unique<LoadBalancerServerPseudo>("[LoadBalancerPseudo]");
                    break;
                case '1':
                    load_balancer = std::make_unique<LoadBalancerServerRoundRobin>("[LoadBalancerRoundRobin]");
                    break;
                default:
                    break;
            }
        } else {
            load_balancer = std::make_unique<LoadBalancerServerPseudo>("[LoadBalancerPseudo]");
        }

        load_balancer->DEBUG_PushServers();
        load_balancer->StartLoadBalancerServer();
        load_balancer->LoadBalancing();
    } catch (const std::exception& e) {
        spdlog::critical("{} {}", instance_name, e.what());
        return EXIT_FAILURE;
    }
}
