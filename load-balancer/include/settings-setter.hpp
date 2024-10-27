#ifndef SETTINGS_SETTER_HPP
#define SETTINGS_SETTER_HPP

#include <memory>

#include "iparser-factory.hpp"
#include "load-balancer-creator.hpp"
#include "load-balancer-mods.hpp"
#include "parser-types.hpp"
#include "setup-info.hpp"

class SettingsSetter {
  public:
    SettingsSetter(std::shared_ptr<LoadBalancerServerInterface> &load_balancer,
                   const std::shared_ptr<IParserFactroy> &iparser_factory, const std::string &file_path);

    void ApplySettings();

  private:
    void GetSettingsFromParser();

  private:
    std::shared_ptr<LoadBalancerServerInterface> &load_balancer_;
    const std::shared_ptr<IParserFactroy> iparser_factory_;
    std::shared_ptr<IParser> parser_;
    LoadBalancerCreator load_balancer_creator_;
    SetupInfo setup_info_;
};

#endif