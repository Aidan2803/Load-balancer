#ifndef TXT_PARSER_FACTORY_HPP
#define TXT_PARSER_FACTORY_HPP

#include "parser/iparser-factory.hpp"
#include "parser/txt-parser.hpp"

class TXTParserFactory : public IParserFactroy {
  public:
    virtual std::shared_ptr<IParser> CreateParser(const std::string& file_path) const override {
        return std::make_shared<TXTParser>(file_path);
    }
};

#endif