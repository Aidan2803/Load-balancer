#ifndef IPARSER_FACTORY_HPP
#define IPARSER_FACTORY_HPP

#include <memory>

#include "parser/iparser.hpp"

class IParserFactroy {
  public:
    virtual ~IParserFactroy() = default;
    virtual std::shared_ptr<IParser> CreateParser(const std::string& file_path) const = 0;
};

#endif