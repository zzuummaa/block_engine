#ifndef BLOCK_ENGINE_CALC_ENGINE_H
#define BLOCK_ENGINE_CALC_ENGINE_H

#include <iostream>
#include <iterator>
#include <utility>

#include "model/scheme.h"
#include "bus_factory.h"
#include "block_factory.h"
#include "block_management_logic.h"
#include "scheme_presenter.h"
#include "graph.h"

namespace block_engine::core {

class CalcEngine {
public:
    explicit CalcEngine(const model::Scheme& scheme, std::shared_ptr<ICoreApiServer> core_api_server)
        : core_api_server(std::move(core_api_server)) {
        setScheme(scheme);
    };

    bool start() {
        return true;
    }

    bool stop() {
        return true;
    }

    bool process_step() {
        for (auto& block_logic : ordered_block_logics) {
            if (!block_logic->calc()) {
                core_api_server->notifyEvent({
                    .block_id = block_logic->block.id,
                    .event_type_id = 0
                });
                return false;
            }
        }

        return true;
    }

private:
    BlockManagementLogic block_management_logic;
    std::vector<int> calc_order;
    std::vector<BlockLogicBasePtr> ordered_block_logics;
    std::shared_ptr<ICoreApiServer> core_api_server;

    void setScheme(const model::Scheme& scheme) {
        block_management_logic.setBlocks(std::move(DefaultBlockPolicy(make_block_factory()).blocks(scheme)));
        block_management_logic.setConnectors(std::move(DefaultConnectionPolicy(make_bus_factory()).connectors(scheme)));
        block_management_logic.connectBlocks();

        calc_order = graph::topologySort(scheme);

        ordered_block_logics.clear();
        std::transform(
            calc_order.begin(),
            calc_order.end(),
            std::inserter(ordered_block_logics, ordered_block_logics.end()),
            [&](int id){ return block_management_logic.getBlock(id); });

        std::copy(calc_order.begin(), calc_order.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }
};

}

#endif //BLOCK_ENGINE_CALC_ENGINE_H
