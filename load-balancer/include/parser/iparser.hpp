#ifndef IPARSER_HPP
#define IPARSER_HPP

#include "utility/setup/setup-info.hpp"

class IParser {
  public:
    virtual SetupInfo GetSetupInfo() = 0;

  protected:
    IParser(const std::string& file_path) : file_path_{file_path} {}

    const std::string& file_path_;
    SetupInfo setup_info_;
};

#endif