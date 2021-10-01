#ifndef BLOCK_ENGINE_CONSOLE_BLOCK_H
#define BLOCK_ENGINE_CONSOLE_BLOCK_H

#include <ostream>
#include "block.h"

template<typename InType>
class ConsoleBlock : IBlock {
    std::ostream os;
    InType& input;
public:

    explicit ConsoleBlock(const std::ostream& os) : os(os) { }

    bool calc() override {
        return os << input;
    }

    void connectInputs(Connector &connector) override { }

    void connectOutputs(Connector &connector) override {
        auto& busses = connector.getBusses();
        busses[0] = value;
    }

    bool validateInputs(const Connector &connector) override {
        // TODO check buss type
        return connector.size() == 1;
    }

    bool validateOutputs(const Connector &connector) override {
        return true;
    }
};

#endif //BLOCK_ENGINE_CONSOLE_BLOCK_H
