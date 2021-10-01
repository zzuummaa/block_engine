//
// Created by Stepan on 30.09.2021.
//

#ifndef BLOCK_ENGINE_CONST_BLOCK_H
#define BLOCK_ENGINE_CONST_BLOCK_H

#include "block.h"

template<typename OutType>
class ConstBlock : IBlock {
    OutType value;
public:

    explicit ConstBlock(const OutType& value) : value(value) {}

    bool calc() override {
        std::accumulate(inputs.begin(), inputs.end(), 0);
        return true;
    }

    void connectInputs(Connector &connector) override { }

    void connectOutputs(Connector &connector) override {
        auto& busses = connector.getBusses();
        busses[0] = value;
    }

    bool validateInputs(const Connector &connector) override {
        return connector.getBusses().empty();
    }

    bool validateOutputs(const Connector &connector) override {
        // TODO check buss type
        return connector.getBusses().size() == 1;
    }
};

#endif //BLOCK_ENGINE_CONST_BLOCK_H
