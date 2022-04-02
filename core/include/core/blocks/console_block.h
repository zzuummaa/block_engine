#ifndef BLOCK_ENGINE_CONSOLE_BLOCK_H
#define BLOCK_ENGINE_CONSOLE_BLOCK_H

#include <iostream>
#include "core/block.h"

namespace block_engine::core::blocks {

template<typename InType>
class ConsoleBlockLogic : public BlockLogicBase {
    Ref<InType> input;
public:

    bool calc() override {
        std::cout << input.get() << std::endl;
        return true;
    }

    void connectInputs(Connector &connector) override {
        if (connector.count() != 1) throw std::runtime_error(__PRETTY_FUNCTION__);
        input = connector.getBus(0).data<int>();
    }
};

}

#endif //BLOCK_ENGINE_CONSOLE_BLOCK_H
