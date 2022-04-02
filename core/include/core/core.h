//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_CORE_H
#define BLOCK_ENGINE_CORE_H

#include <utility>

#include "core_api.h"
#include "calc_engine.h"
#include "execution_logic.h"

namespace block_engine::core {

class Core : public ICoreApiServerHandler {
public:
    explicit Core(std::shared_ptr<ICoreApiServer> core_api_server) : core_api_server(std::move(core_api_server)) {}

    void onSetScheme(const model::Scheme &scheme) override {
        if (is_calcing) throw std::invalid_argument(__PRETTY_FUNCTION__);
        calc_engine = std::make_shared<CalcEngine>(scheme, core_api_server);
    }

    void onStartCalc() override {
        if (!calc_engine) throw std::invalid_argument(__PRETTY_FUNCTION__);
        execution_logic.start(calc_engine);
        is_calcing = true;
    }

    void onStopCalc() override {
        execution_logic.stop();
        is_calcing = false;
    }

private:
    std::shared_ptr<ICoreApiServer> core_api_server;
    std::shared_ptr<CalcEngine> calc_engine;
    SingleSeparateThreadExecutionLogic execution_logic;
    bool is_calcing{};
};

}

#endif //BLOCK_ENGINE_CORE_H
