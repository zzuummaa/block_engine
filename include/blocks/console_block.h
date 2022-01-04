#ifndef BLOCK_ENGINE_CONSOLE_BLOCK_H
#define BLOCK_ENGINE_CONSOLE_BLOCK_H

#include <iostream>
#include "block.h"

template<typename InType>
class ConsoleBlock : public IBlock {
    Ref<InType> input;
public:

    bool calc() override {
        std::cout << input.get();
        return true;
    }

    void connectInputs(Connector &connector) override {
        if (connector.count() != 1) throw std::runtime_error(__PRETTY_FUNCTION__);
        input = connector.getBus(0).data<int>();
    }

    void connectOutputs(Connector &connector) override { }

};

#endif //BLOCK_ENGINE_CONSOLE_BLOCK_H
