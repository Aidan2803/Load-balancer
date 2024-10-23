#include "load-balancer-interface.hpp"
#include <memory>

class SettingsSetter{
    public:
    SettingsSetter(const std::shared_ptr<LoadBalancerServerInterface> &load_balancer, int parser_type);
};