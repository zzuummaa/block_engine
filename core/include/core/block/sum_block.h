//
// Created by Stepan on 26.09.2021.
//

#ifndef MODERN_CPP_DESIGN_SUM_BLOCK_H
#define MODERN_CPP_DESIGN_SUM_BLOCK_H

#include <numeric>
#include "block.h"

namespace block_engine::core::block {

template<typename OperandType>
class SumBlockLogic : public BlockLogicBase {
    std::vector<Ref<OperandType>> inputs;
    Ref<OperandType> output;

    OperandType dummy;

public:

    bool calc() override {
        output.get() = std::accumulate(
            inputs.begin() + 1,
            inputs.end(),
            inputs.begin()->get(),
            [](const auto& a, const auto& b){ return a + b.get(); });
        return true;
    }

    void connectInputs(Connector &connector) override {
        if (connector.count() == 0) {
            inputs.resize(1);
            inputs[0] = dummy;
        }
        inputs.resize(connector.count());

        for (size_t i = 0; i < connector.count(); i++) {
            auto& bus = connector.getBus(i);
            inputs[i] = bus.data<OperandType>();
        }
    }

    void connectOutputs(Connector &connector) override {
        if (connector.count() != 1) throw std::runtime_error(__PRETTY_FUNCTION__);
        output = connector.getBus(0).data<OperandType>();
    }
};

}

#endif //MODERN_CPP_DESIGN_SUM_BLOCK_H
