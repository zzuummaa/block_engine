#include "model/bus.h"

namespace block_engine::model {

bool operator<(const Pin &lhs, const Pin &rhs) {
    if (lhs.block_id < rhs.block_id)
        return true;
    if (rhs.block_id < lhs.block_id)
        return false;
    return lhs.pin_idx < rhs.pin_idx;
}

}