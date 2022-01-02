#include <memory>
#include <fstream>
#include <string>

#include <blocks/sum_block.h>
#include <blocks/const_block.h>
#include <blocks/console_block.h>

#include <json_scheme_parser.h>
#include <scheme_validator.h>
#include <graph.h>

class SimpleBlockFactory {
public:
    static std::shared_ptr<IBlock> createBlockByName(const std::string& name) {
        if (name == "const_int") {
            return std::static_pointer_cast<IBlock>(std::make_shared<ConstBlock<int>>(3));
        } else if (name == "sum_2_int") {
            return std::static_pointer_cast<IBlock>(std::make_shared<SumBlock<int>>());
        } else if (name == "console_int") {
            return std::static_pointer_cast<IBlock>(std::make_shared<ConsoleBlock<int>>());
        }

        throw std::runtime_error(__PRETTY_FUNCTION__);
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
    std::fstream fs("test.json");
    auto scheme = JsonSchemeParser(fs).parse();
    if (!SchemeValidator().validate(scheme)) throw std::runtime_error(__PRETTY_FUNCTION__);

    auto calc_order = graph::topologySort(scheme);

    std::copy(calc_order.begin(), calc_order.end(), std::ostream_iterator<int>(std::cout, " "));

    return 0;
}