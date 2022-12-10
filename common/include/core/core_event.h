//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_EVENT_H
#define BLOCK_ENGINE_EVENT_H

#include <iostream>

namespace block_engine::core {

enum class EventType {
    Core,
};

struct Event {
    EventType type;

    Event(EventType type) : type(type) {}
};

enum class CoreEventSubType {
    Start,
    Stop
};

struct CoreEvent : Event {
    int block_id;
    CoreEventSubType sub_type;

    CoreEvent(int block_id, CoreEventSubType sub_type) : Event(EventType::Core), block_id(block_id), sub_type(sub_type) {}
};

std::ostream &operator<<( std::ostream &output, const CoreEventSubType sub_type) {
    switch (sub_type) {
        case CoreEventSubType::Start:
            output << "Start";
            break;
        case CoreEventSubType::Stop:
            output << "Stop";
            break;
        default:
            output << static_cast<int>(sub_type);
    }
    return output;
}

std::ostream &operator<<( std::ostream &output, const CoreEvent& event) {
    output << "CoreEvent{block_id=" << event.block_id << ", sub_type=" << event.sub_type << "}";
    return output;
}

}

#endif //BLOCK_ENGINE_EVENT_H
