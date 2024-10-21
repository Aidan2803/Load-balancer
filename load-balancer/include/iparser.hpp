template <typename T>
class IParser {
  protected:
    IParser(const T& file_path) : file_path_{file_path} {}
    virtual SetupInfo GetSetupInfo() = 0;
    const T& file_path_;
    SetupInfo setup_info_;
};