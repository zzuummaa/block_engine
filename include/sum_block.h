//
// Created by Stepan on 26.09.2021.
//

#ifndef MODERN_CPP_DESIGN_SUM_BLOCK_H
#define MODERN_CPP_DESIGN_SUM_BLOCK_H

#include <numeric>
#include "block.h"

template<typename OperandType>
class SumBlock : IBlock {
    std::vector<OperandType&> inputs;
    OperandType& output;

public:

    bool calc() override {
        std::accumulate(inputs.begin(), inputs.end(), 0);
        return true;
    }

    void connectInputs(Connector &connector) override {
        inputs.resize(connector.size());
        for (size_t i = 0; i < connector.size(); i++) {
            auto& bus = connector[i];
            inputs[i] = bus.data<int&>();
        }
    }

    void connectOutputs(Connector &connector) override {
        output.resize(1);
        output[0] = connector[0];
    }

    bool validateInputs(const Connector &connector) override {
        return true;
    }

    bool validateOutputs(const Connector &connector) override {
        return connector.size() == 1;
    }
};

#endif //MODERN_CPP_DESIGN_SUM_BLOCK_H
