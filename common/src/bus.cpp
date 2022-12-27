#include <stdexcept>

#include "model/bus.h"

namespace block_engine::model {

PinId::PinId(int blockId, bool isInput, int pinIdx) {
    if (pinIdx < 0 || pinIdx >= 1 << 30 || blockId < 0) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
    id = static_cast<uint64_t>(blockId) << 31 | isInput << 30 | pinIdx;
}

int PinId::getBlockId() const {
    return static_cast<int>(id >> 31);
}

int PinId::getPinIdx() const {
    return static_cast<int>(id & ~(0x1FFFFll << 30));
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