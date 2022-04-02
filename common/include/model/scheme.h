//
// Created by Stepan on 31.12.2021.
//

#ifndef BLOCK_ENGINE_SCHEME_H
#define BLOCK_ENGINE_SCHEME_H

#include <map>
#include <set>
#include <string>

#include "block.h"
#include "bus.h"

namespace block_engine::model {

struct Scheme {
    std::map<int, std::string> types;
    std::map<int, std::string> block_types;
    std::map<int, Block> blocks;
    std::set<Bus, BusLessByBlockId> busses;

    bool addType(int type_id, const std::string& type);
    [[nodiscard]] std::optional<std::string> getType(int type_id) const;
    bool addBlockType(int block_id, const std::string& block_type);
    [[nodiscard]] std::optional<std::string> getBlockType(int block_type_id) const;
    bool addBlock(int block_id, const Block& block);
    [[nodiscard]] std::optional<Block> getBlock(int block_id) const;
    bool addBus(const Bus& link);
};

}

#endif //BLOCK_ENGINE_SCHEME_H
