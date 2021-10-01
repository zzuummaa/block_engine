#include <json_scheme_parser.h>
#include <fstream>

class SimpleBlockFactory : IBlockFactory {
    IBlock* createBlockByName() override {
        return nullptr;
    }
};

int main() {
    std::fstream fs("test.json");


    return 0;
}