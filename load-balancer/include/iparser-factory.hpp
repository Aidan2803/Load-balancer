#include "iparser.hpp"
#include <memory>

class IParserFactroy{
    public:
    virtual ~IParserFactroy() = default;
    virtual std::shared_ptr<IParser> CreateParser() = 0;
};