//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_BUS_H
#define BLOCK_ENGINE_BUS_H

#include <cstdint>
#include <vector>

namespace block_engine::model {

class PinId;

using TPinId = PinId;
using TPinTypeId = int;

class PinId {
    int64_t id;

public:
    explicit PinId(long id) : id(id) {}

    PinId(int blockId, bool isInput, int pinIdx);

//    PinId(const PinId&) = default;
//
//    PinId& operator=(const PinId&) = default;

    [[nodiscard]] int getBlockId() const {
        return static_cast<int>(id >> 31);
    }

    [[nodiscard]] int getPinIdx() const {
        return static_cast<int>(id & ~(0x1FFFFll << 30));
    }

    friend bool operator<(const PinId& lhs, const PinId& rhs);
};

struct Pin {
    TPinId id;
    TPinTypeId typeId;
};

struct Link {
    TPinId output;
    TPinId input;
};

struct Bus {
    int type_id{};
    Pin src;
    std::vector<Pin> dests;
};

bool operator<(const Pin& lhs, const Pin& rhs);

bool operator<(const Link& lhs, const Link& rhs);

}

#endif //BLOCK_ENGINE_BUS_H
