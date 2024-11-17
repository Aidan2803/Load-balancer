#include <signal.h>

#include "balancer/load-balancer-pseudo.hpp"
#include "balancer/load-balancer-roundrobin.hpp"
#include "parser/json-parser-factory.hpp"
#include "parser/parser-types.hpp"
#include "parser/txt-parser-factory.hpp"
#include "spdlog/spdlog.h"
#include "utility/settup/settings-setter.hpp"

void PrintHelp() {
    spdlog::info("Usage:");
    spdlog::info("./load-balancer <settings-file.json/txt>");
}

ParserTypes AnalyzeFileType(char* file_name) {
    std::string file_name_str(file_name);

    std::size_t file_ext_index = file_name_str.rfind(".json");
    if (file_ext_index == file_name_str.size() - 5) {
        ParserTypes parser_type = ParserTypes::JSONParser;
        return parser_type;
    }

    file_ext_index = file_name_str.rfind(".txt");
    if (file_ext_index == file_name_str.size() - 4) {
        ParserTypes parser_type = ParserTypes::TXTParser;
        return parser_type;
    } else {
        throw std::runtime_error("Unknown file extension " + file_name_str);
    }
}

int main(int argc, char** argv) {
    signal(SIGPIPE, SIG_IGN);  // Ignore SIGPIPE
    spdlog::set_level(spdlog::level::debug);

    const std::string instance_name{"[Main]"};

    try {
        if (argc < 2) {
            PrintHelp();
            throw(std::runtime_error("No settings file was provided!"));
        }

        ParserTypes parser_type = AnalyzeFileType(argv[1]);

        std::shared_ptr<LoadBalancerServerInterface> load_balancer;
        std::shared_ptr<IParserFactroy> iparser_factory;

        if (parser_type == ParserTypes::JSONParser) {
            iparser_factory = std::make_shared<JSONParserFactory>();
        } else if (parser_type == ParserTypes::TXTParser) {
            iparser_factory = std::make_shared<TXTParserFactory>();
        }

        SettingsSetter settings_setter(load_balancer, iparser_factory, static_cast<std::string>(argv[1]));

        settings_setter.ApplySettings();

        // load_balancer->DEBUG_PushServers();

        if (load_balancer) {
            load_balancer->StartLoadBalancerServer();
            load_balancer->LoadBalancing();
        }
    } catch (const std::exception& e) {
        spdlog::critical("{} {}", instance_name, e.what());
        return EXIT_FAILURE;
    }
}
