#ifndef BLOCK_ENGINE_CALC_ENGINE_H
#define BLOCK_ENGINE_CALC_ENGINE_H

#include <iostream>
#include <iterator>
#include <utility>
#include <atomic>

#include "model/scheme.h"
#include "bus_factory.h"
#include "block_factory.h"
#include "block_management_logic.h"
#include "scheme_presenter.h"
#include "graph.h"

namespace block_engine::core {

struct ICalcEngineEventHandler {
    /**
     * Used for notify about Scheme validation and other unexpected situations.
     */
    virtual void notifyError(const CoreError& error) = 0;

    /**
     * Used for notify about important calculation events.
     */
    virtual void notifyEvent(const CoreEvent& event) = 0;
};

struct CoutPrintEventHandler : public ICalcEngineEventHandler {
    std::shared_ptr<std::atomic_bool> is_end;

    explicit CoutPrintEventHandler(std::shared_ptr<std::atomic_bool> is_end) : is_end(std::move(is_end)) {
        *this->is_end = true;
    }

    virtual ~CoutPrintEventHandler() = default;

    void notifyError(const CoreError& error) override {
        std::cout << error << std::endl;
    }

    void notifyEvent(const CoreEvent& event) override {
        std::cout << event << std::endl;
        switch (event.sub_type) {
            case CoreEventSubType::Start:
                *is_end = false;
                break;
            case CoreEventSubType::Stop:
                *is_end = true;
                break;
        }
    }
};

class CalcEngine {
public:
    explicit CalcEngine(const model::Scheme& scheme, std::shared_ptr<ICalcEngineEventHandler> event_handler)
        : event_handler(std::move(event_handler)) {
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
                event_handler->notifyEvent({
                    block_logic->block.id,
                    CoreEventSubType::Stop
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
    std::shared_ptr<ICalcEngineEventHandler> event_handler;

    void setScheme(const model::Scheme& scheme) {
        block_management_logic.setBlocks(DefaultBlockPolicy(makeBlockFactory()).blocks(scheme));
        block_management_logic.setConnectors(DefaultConnectionPolicy(makeBusFactory()).connectors(scheme));
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
