#include <memory>
#include <fstream>
#include <string>

#include <blocks/sum_block.h>
#include <blocks/const_block.h>
#include <blocks/console_block.h>

#include <json_scheme_parser.h>
#include <scheme_validator.h>
#include <graph.h>
#include <block_connection_policy.h>
#include <block_management_logic.h>

class SimpleBlockFactory {
public:
    static IBlock* createBlockByName(const std::string& name) {
        if (name == "const_int") {
            return new ConstBlock<int>(3);
        } else if (name == "sum_2_int") {
            return new SumBlock<int>;
        } else if (name == "console_int") {
            return new ConsoleBlock<int>();
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

struct SimpleBusFactory {
    static Bus createBus(const std::string& type_name) {
        if (type_name == "int") {
            return Bus(int());
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
    std::cout << std::endl;

    BlockManagementLogic block_management_logic;
    block_management_logic.setBlocks(std::move(DefaultBlockPolicy<SimpleBlockFactory>::blocks(scheme)));
    block_management_logic.setConnectors(std::move(DefaultConnectionPolicy<SimpleBusFactory>::connectors(scheme)));
    block_management_logic.connectBlocks();

    for (const auto& id: calc_order) {
        block_management_logic.getBlock(id)->calc();
    }

    return 0;
}