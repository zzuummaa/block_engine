#pragma once

#include <iostream>
#include "block.h"

namespace block_engine::core::block {

template<typename TInType>
class ConsoleBlockLogic : public BlockLogicBase {
    Ref<TInType> input;
public:

    bool calc() override {
        std::cout << input.get() << std::endl;
        return true;
    }

    void connectInputs(Connector &connector) override {
        if (connector.count() != 1) throw std::runtime_error(__PRETTY_FUNCTION__);
        input = connector.getBus(0).data<TInType>();
    }
};

}
