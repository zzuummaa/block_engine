//
// Created by Stepan on 30.09.2021.
//

#ifndef BLOCK_ENGINE_CONST_BLOCK_H
#define BLOCK_ENGINE_CONST_BLOCK_H

#include "core/block.h"

namespace block_engine::core::blocks {

template<typename OutType>
class ConstBlockLogic : public BlockLogicBase {
    OutType value;
    Ref<OutType> output;
public:

    explicit ConstBlockLogic(const OutType& value) : value(value) {}

    bool calc() override {
        output.get() = value;
        return true;
    }

    void connectInputs(Connector &connector) override {
        if (connector.count() != 0) throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    void connectOutputs(Connector &connector) override {
        if (connector.count() != 1) throw std::runtime_error(__PRETTY_FUNCTION__);
        output = connector.getBus(0).data<OutType>();
    }
};

}

#endif //BLOCK_ENGINE_CONST_BLOCK_H
