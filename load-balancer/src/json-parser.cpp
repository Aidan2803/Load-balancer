#include "json-parser.hpp"

JSONParser::JSONParser(std::string& file_path) : IParser(file_path) {}

void JSONParser::ReadFromFile() {
    // TODO:check if file is actually json
    std::ifstream input_file(file_path_);

    if (!input_file.is_open()) {
        spdlog::error("Could not open JSON file\n");
        throw std::runtime_error("Could not open JSON file");
    }
    input_file >> json_data_;
    input_file.close();
}

void JSONParser::ParseJSON() {
    if (json_data_.contains("servers_amount")) {
        setup_info_.servers_amount_ = json_data_["servers_amount"];
    }

    if (json_data_.contains("load_balancing_mode")) {
        setup_info_.load_balancer_mode_ = json_data_["load_balancing_mode"];
    }

    if (json_data_.contains("load_balancing_is_keep_alive")) {
        setup_info_.load_balancing_is_keep_alive = json_data_["load_balancing_is_keep_alive"];
    }

    // TODO: add checks here, what if json file is incorrectly formated
    if (json_data_.contains("servers")) {
        const auto& servers = json_data_["servers"];

        for (auto& server : servers) {
            setup_info_.server_ip_port_.emplace_back(static_cast<std::string>(server["ip"]),
                                                     static_cast<std::string>(server["port"]), false);
        }
    }
}

SetupInfo JSONParser::GetSetupInfo(){
    return setup_info_;
};