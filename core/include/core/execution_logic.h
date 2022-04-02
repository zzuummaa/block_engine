//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_EXECUTION_LOGIC_H
#define BLOCK_ENGINE_EXECUTION_LOGIC_H

#include "calc_engine.h"
#include <thread>
#include <atomic>
#include <utility>

namespace block_engine::core {

struct IExecutionLogic {
    virtual void start(std::shared_ptr<CalcEngine> calcEngine) = 0;
    virtual void stop() = 0;
};

class SingleSeparateThreadExecutionLogic : public IExecutionLogic {
public:
    void start(std::shared_ptr<CalcEngine> engine) override {
        if (thread.joinable()) throw std::runtime_error(__PRETTY_FUNCTION__);

        is_running = true;
        thread = std::thread([this, engine](){
            engine->start();
            while (is_running) {
                if (!engine->process_step()) {
                    is_running = false;
                    break;
                }
            }
            engine->stop();
        });
    }

    void stop() override {
        is_running = false;
        thread.join();
    }

private:
    std::thread thread;
    std::atomic_bool is_running{};
};

}

#endif //BLOCK_ENGINE_EXECUTION_LOGIC_H
