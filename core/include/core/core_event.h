//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_EVENT_H
#define BLOCK_ENGINE_EVENT_H

namespace block_engine::core {

struct CoreEvent {
    int block_id{};
    int event_type_id{};
};

std::ostream &operator<<( std::ostream &output, const CoreEvent& event) {
    output << "CoreEvent{block_id=" << event.block_id << ", event_type_id=" << event.event_type_id << "}";
    return output;
}

}

#endif //BLOCK_ENGINE_EVENT_H
