#include "parser/json-parser.hpp"

JSONParser::JSONParser(const std::string& file_path) : IParser(file_path) {
    try {
        ReadFromFile();
        ParseJSON();
    } catch (std::exception& e) {
        throw;
    }
}

void JSONParser::ReadFromFile() {
    std::ifstream input_file(file_path_);

    if (!input_file.is_open()) {
        spdlog::error("Could not open JSON file\n");
        throw std::runtime_error("Could not open JSON file");
    }
    input_file >> json_data_;
    input_file.close();
}

// TODO: add checks here, what if json file is incorrectly formated
// TODO: make cheks, make cheks for valid ip:port for load-balancer and servers
void JSONParser::ParseJSON() {
    if (json_data_.contains("servers_amount")) {
        setup_info_.servers_amount_ = json_data_["servers_amount"];
    }

    if (json_data_.contains("load_balancer_mode")) {
        if (json_data_["load_balancer_mode"] == "round_robin") {
            setup_info_.load_balancer_mode_ = LoadBalancerMods::RoundRobin;
        } else if (json_data_["load_balancer_mode"] == "pseudo") {
            setup_info_.load_balancer_mode_ = LoadBalancerMods::Pseudo;
        } else {
            throw std::runtime_error(
                std::string("Unknwon load-balancer mode " + std::string(json_data_["load_balancer_mode"])));
        }
    }

    if (json_data_.contains("load_balancer_ip")) {
        setup_info_.load_balancer_ip_ = json_data_["load_balancer_ip"];
    }

    if (json_data_.contains("load_balancer_port")) {
        setup_info_.load_balancer_port_ = json_data_["load_balancer_port"];
    }

    if (json_data_.contains("load_balancing_is_keep_alive")) {
        setup_info_.load_balancing_is_keep_alive = json_data_["load_balancing_is_keep_alive"];
    }

    if (json_data_.contains("load_balancer_backlog_size")) {
        setup_info_.load_balancer_backlog_size_ = json_data_["load_balancer_backlog_size"];
    }

    if (json_data_.contains("servers")) {
        const auto& servers = json_data_["servers"];

        if (setup_info_.load_balancer_mode_ == LoadBalancerMods::Pseudo) {
            setup_info_.servers_amount_ = 1;
            setup_info_.server_ip_port_.emplace_back(static_cast<std::string>(servers[0]["ip"]),
                                                     static_cast<std::string>(servers[0]["port"]), true);
        }

        for (auto& server : servers) {
            setup_info_.server_ip_port_.emplace_back(static_cast<std::string>(server["ip"]),
                                                     static_cast<std::string>(server["port"]), true);
        }
    }
}

SetupInfo JSONParser::GetSetupInfo() { return setup_info_; };