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
        : eventHandler(std::move(event_handler)) {
        setScheme(scheme);
    };

    bool start() {
        bool isOk = true;
        for (auto& blockLogic : orderedBlockLogics) {
            isOk &= blockLogic->start();
        }
        return isOk;
    }

    bool stop() {
        bool isOk = true;
        for (auto& blockLogic : orderedBlockLogics) {
            isOk &= blockLogic->start();
        }
        return isOk;
    }

    bool process_step() {
        for (auto& blockLogic : orderedBlockLogics) {
            if (!blockLogic->calc()) {
                eventHandler->notifyEvent({
                    blockLogic->block.id,
                    CoreEventSubType::Stop
                });
                return false;
            }
        }

        return true;
    }

private:
    BlockManagementLogic blockManagementLogic;
    std::vector<int> calcOrder;
    std::vector<BlockLogicBasePtr> orderedBlockLogics;
    std::shared_ptr<ICalcEngineEventHandler> eventHandler;

    void setScheme(const model::Scheme& scheme) {
        blockManagementLogic.setBlocks(DefaultBlockPolicy(makeBlockFactory()).blocks(scheme));
        blockManagementLogic.setConnectors(DefaultConnectionPolicy(makeBusFactory()).connectors(scheme));
        blockManagementLogic.connectBlocks();

        calcOrder = graph::topologySort(scheme);

        orderedBlockLogics.clear();
        std::transform(
            calcOrder.begin(),
            calcOrder.end(),
            std::inserter(orderedBlockLogics, orderedBlockLogics.end()),
            [&](int id){ return blockManagementLogic.getBlock(id); });

        std::copy(calcOrder.begin(), calcOrder.end(), std::ostream_iterator<int>(std::cout, " "));
        std::cout << std::endl;
    }
};

}

#endif //BLOCK_ENGINE_CALC_ENGINE_H
