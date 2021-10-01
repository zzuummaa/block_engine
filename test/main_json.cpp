#include <fstream>
#include <string>

#include <blocks/sum_block.h>
#include <blocks/const_block.h>
#include <blocks/console_block.h>

#include <json_scheme_parser.h>

class SimpleBlockFactory {
public:
    static IBlock* createBlockByName(const std::string& name, DataStorage& blockStorage) {
        void* block = nullptr;
        if (name == "const_int") {
            block = blockStorage.push_back(ConstBlock<int>(3)).data();
        } else if (name == "sum_2_int") {
            block = blockStorage.push_back(SumBlock<int>()).data();
        } else if (name == "console_int") {
            block = blockStorage.push_back(ConsoleBlock<int>()).data();
        }

        if (block == nullptr) throw std::runtime_error(__PRETTY_FUNCTION__);
        return reinterpret_cast<IBlock*>(block);
    }
};

class SimpleTypeFactory {
public:
    static TypeInfo createTypeInfoByName(const std::string& name) {
        if (name == "int") {
            return TypeInfo(typeid(int), sizeof(int));
        }
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
};

int main() {
    DataStorage blockStorage;
    LinkEngine linkEngine;

    std::fstream fs("test.json");
    JsonSchemeParser<SimpleBlockFactory, SimpleTypeFactory> jsonSchemeParser(fs);
    jsonSchemeParser.parse(linkEngine, blockStorage);

    linkEngine.linkBlocks();


    return 0;
}