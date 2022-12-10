//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_CORE_API_H
#define BLOCK_ENGINE_CORE_API_H

#include "model/scheme.h"
#include "core_error.h"
#include "core/core_event.h"

namespace block_engine::core {

struct ICoreApiServer {
    /**
     * Used for notify about Scheme validation and other unexpected situations.
     */
    virtual void notifyError(const CoreError& error) = 0;

    /**
     * Used for notify about important calculation events.
     */
    virtual void notifyEvent(const CoreEvent& event) = 0;
};

struct ICoreApiServerHandler {
    virtual void onSetScheme(const model::Scheme& scheme) = 0;
    virtual void onStartCalc() = 0;
    virtual void onStopCalc() = 0;
};

struct ICoreApiClient {
    virtual void notifySetScheme(const model::Scheme& scheme) = 0;
    virtual void notifyStartCalc() = 0;
    virtual void notifyStopCalc() = 0;
};

struct ICoreApiClientHandler {
    virtual void onError(const CoreError& error) = 0;
    virtual void onEvent(const CoreEvent& event) = 0;
};

}

#endif //BLOCK_ENGINE_CORE_API_H
