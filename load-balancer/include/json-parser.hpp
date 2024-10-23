#include <fstream>
#include <nlohmann/json.hpp>

#include "iparser.hpp"
#include "spdlog/spdlog.h"

class JSONParser : public IParser {
    JSONParser(std::string &file_path);
    virtual SetupInfo GetSetupInfo() override;

  private:
    void ReadFromFile();
    void ParseJSON();

  private:
    nlohmann::json json_data_;
};
