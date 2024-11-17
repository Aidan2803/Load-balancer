#ifndef JSON_PARSER_HPP
#define JSON_PARSER_HPP

#include <fstream>
#include <nlohmann/json.hpp>

#include "parser/iparser.hpp"
#include "spdlog/spdlog.h"

class JSONParser : public IParser {
  public:
    JSONParser(const std::string &file_path);
    virtual SetupInfo GetSetupInfo() override;

  private:
    void ReadFromFile();
    void ParseJSON();

  private:
    nlohmann::json json_data_;
};

#endif