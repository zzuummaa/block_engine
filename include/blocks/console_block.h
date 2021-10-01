#ifndef BLOCK_ENGINE_CONSOLE_BLOCK_H
#define BLOCK_ENGINE_CONSOLE_BLOCK_H

#include <iostream>
#include "block.h"

template<typename InType>
class ConsoleBlock : IBlock {
    InType* input;
public:

    bool calc() override {
        std::cout << *input;
        return true;
    }

    void connectInputs(Connector &connector) override {
        input = &connector.getBusses()[0].data_unchecked<int>();
    }

    void connectOutputs(Connector &connector) override { }

    bool validateInputs(const Connector &connector) override {
        // TODO check buss type
        return connector.getBusses().size() == 1;
    }

    bool validateOutputs(const Connector &connector) override {
        return true;
    }
};

#endif //BLOCK_ENGINE_CONSOLE_BLOCK_H
