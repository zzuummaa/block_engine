//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_BUS_H
#define BLOCK_ENGINE_BUS_H

#include <vector>

namespace block_engine::model {

struct Pin {
    int block_id{};
    int pin_idx{};
};

struct Bus {
    int type_id{};
    Pin src;
    std::vector<Pin> dests;
};

bool operator<(const Pin &lhs, const Pin &rhs);

struct BusLessByBlockId {
    bool operator()(const Bus& lhs, const Bus& rhs) const {
        if (lhs.type_id < rhs.type_id)
            return true;
        if (rhs.type_id < lhs.type_id)
            return false;
        if (lhs.src < rhs.src)
            return true;
        if (rhs.src < lhs.src)
            return false;
        return lhs.dests < rhs.dests;
    }
};

}

#endif //BLOCK_ENGINE_BUS_H
