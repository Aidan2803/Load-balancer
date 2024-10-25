#ifndef JSON_PARSER_FACTORY_HPP
#define JSON_PARSER_FACTORY_HPP

#include "iparser-factory.hpp"
#include "json-parser.hpp"

class JSONParserFactory : public IParserFactroy {
  public:
    virtual std::shared_ptr<IParser> CreateParser(const std::string& file_path) const override {
        return std::make_shared<JSONParser>(file_path);
    }
};

#endif