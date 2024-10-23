#include "iparser-factory.hpp"
#include "json-parser.hpp"

class JSONParserFactory: public IParserFactroy{
    public:
    virtual std::shared_ptr<IParser> CreateParser() override{
        return std::make_shared<JSONParser>();
    }
};