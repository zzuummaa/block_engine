#ifndef BLOCK_ENGINE_CORE_H
#define BLOCK_ENGINE_CORE_H

#include <iostream>
#include <iterator>

#include "model/scheme.h"
#include "bus_factory.h"
#include "block_factory.h"
#include "block_management_logic.h"
#include "scheme_presenter.h"
#include "graph.h"

namespace block_engine::core {

class CalcEngine {
public:
    explicit CalcEngine(const model::Scheme& scheme) {
        block_management_logic.setBlocks(std::move(DefaultBlockPolicy(make_block_factory()).blocks(scheme)));
        block_management_logic.setConnectors(std::move(DefaultConnectionPolicy(make_bus_factory()).connectors(scheme)));
        block_management_logic.connectBlocks();

        calc_order = graph::topologySort(scheme);

        std::copy(calc_order.begin(), calc_order.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }

    bool start() {
        return true;
    }

    bool stop() {
        return true;
    }

    bool process_step() {
        return std::all_of(
        calc_order.begin(),
        calc_order.end(),
        [&](int id){ return block_management_logic.getBlock(id)->calc(); });
    }

private:
    BlockManagementLogic block_management_logic;
    std::vector<int> calc_order;
};

}

#endif //BLOCK_ENGINE_CORE_H
