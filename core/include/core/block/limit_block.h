//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_LIMIT_BLOCK_H
#define BLOCK_ENGINE_LIMIT_BLOCK_H

#include "block.h"

namespace block_engine::core::block {

template<typename InType>
class LimitBlockLogic : public BlockLogicBase {
    int count_before_stop{};
    Ref<InType> input;
public:

    bool start() override {
        count_before_stop = 10;
        return true;
    }

    bool calc() override {
        count_before_stop--;
        return count_before_stop > 0;
    }

    void connectInputs(Connector &connector) override {
        if (connector.count() != 1) throw std::runtime_error(__PRETTY_FUNCTION__);
        input = connector.getBus(0).data<InType>();
    }
};

}

#endif //BLOCK_ENGINE_LIMIT_BLOCK_H
