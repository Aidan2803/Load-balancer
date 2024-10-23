#include "iparser-factory.hpp"
#include "txt-parser.hpp"

class TXTParserFactory: public IParserFactroy{
    public:
    virtual std::shared_ptr<IParser> CreateParser() override{
        return std::make_shared<TXTParser>();
    }
};