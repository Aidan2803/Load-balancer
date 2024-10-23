#include <memory>

#include "load-balancer-interface.hpp"
#include "parser-types.hpp"

class SettingsSetter {
  public:
    SettingsSetter(const std::shared_ptr<LoadBalancerServerInterface> &load_balancer, ParserTypes parser_type);

  private:
    std::shared_ptr<LoadBalancerServerInterface> load_balancer_;
    ParserTypes parser_type_;
};