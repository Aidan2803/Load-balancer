#include "utility/setup/settings-setter.hpp"

SettingsSetter::SettingsSetter(std::shared_ptr<LoadBalancerServerInterface> &load_balancer,
                               const std::shared_ptr<IParserFactroy> &iparser_factory, const std::string &file_path

                               )
    : load_balancer_{load_balancer}, iparser_factory_{iparser_factory} {
    parser_ = iparser_factory_->CreateParser(file_path);
    GetSettingsFromParser();
}

void SettingsSetter::GetSettingsFromParser() {
    if (parser_) {
        setup_info_ = parser_->GetSetupInfo();
    }
}

void SettingsSetter::ApplySettings() {
    load_balancer_ = load_balancer_creator_.CreateLoadBalancer(
        setup_info_.load_balancer_mode_, setup_info_.load_balancer_ip_, setup_info_.load_balancer_port_);

    load_balancer_->SetServersInfo(setup_info_.server_ip_port_);
    load_balancer_->SetIsKeepAliveInfo(setup_info_.load_balancing_is_keep_alive);
}