//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_EXECUTION_LOGIC_H
#define BLOCK_ENGINE_EXECUTION_LOGIC_H

#include "calc_engine.h"
#include <thread>
#include <atomic>
#include <mutex>
#include <utility>

namespace block_engine::core {

struct IExecutionLogic {
    virtual void start(const std::shared_ptr<CalcEngine>& calcEngine) = 0;
    virtual void stop() = 0;
};

class SingleSeparateThreadExecutionLogic : public IExecutionLogic {
public:
    void start(const std::shared_ptr<CalcEngine>& engine) override {
        if (thread.joinable()) throw std::runtime_error(__PRETTY_FUNCTION__);

        is_running->store(true);
        thread = std::thread([this, engine](){
            engine->start();
            while (is_running->load()) {
                if (!engine->process_step()) {
                    is_running->store(false);
                    break;
                }
            }
            // TODO protect from using from several threads
            engine->stop();
        });
    }

    void stop() override {
        is_running->store(false);
        if (thread.joinable()) {
            thread.detach();
        }
    }

    virtual ~SingleSeparateThreadExecutionLogic()
    {
        is_running->store(false);
        if (thread.joinable()) {
            thread.detach();
        }
    }
private:
    std::thread thread;
    std::shared_ptr<std::atomic_bool> is_running = std::make_shared<std::atomic_bool>(false);
};

}

#endif //BLOCK_ENGINE_EXECUTION_LOGIC_H
