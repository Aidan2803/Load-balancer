#ifndef TXT_PARSER_HPP
#define TXT_PARSER_HPP

#include "iparser.hpp"

class TXTParser : public IParser {
  public:
    TXTParser(const std::string& file_path) : IParser(file_path){};
    virtual SetupInfo GetSetupInfo() {}
};

#endif