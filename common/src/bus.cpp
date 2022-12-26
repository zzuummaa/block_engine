#include <stdexcept>

#include "model/bus.h"

namespace block_engine::model {

PinId::PinId(int blockId, bool isInput, int pinIdx) : id(blockId << 31 | isInput << 30 | pinIdx) {
    if (pinIdx < 0) throw std::runtime_error(__PRETTY_FUNCTION__);
}

bool operator<(const PinId& lhs, const PinId& rhs) {
    return lhs.id < rhs.id;
}

bool operator<(const Pin &lhs, const Pin &rhs) {
    return lhs.id < rhs.id;
}

bool operator<(const Link& lhs, const Link& rhs) {
    if (lhs.output < rhs.output) {
        return true;
    }
    if (rhs.output < lhs.output) {
        return false;
    }
    return lhs.input < rhs.input;
}

}