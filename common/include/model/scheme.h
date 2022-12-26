//
// Created by Stepan on 31.12.2021.
//

#ifndef BLOCK_ENGINE_SCHEME_H
#define BLOCK_ENGINE_SCHEME_H

#include <map>
#include <set>
#include <string>

#include "block_type_info.h"
#include "block.h"
#include "bus.h"

namespace block_engine::model {

struct Scheme {
    std::map<TTypeId, std::string> types;
    std::map<TBlockTypeId, std::string> block_types;
    std::map<TPinId, Pin> pins;
    std::map<TBlockId, Block> blocks;
    std::set<Link> links;

    bool addType(TTypeId id, const std::string& type);
    [[nodiscard]] std::optional<std::string> getType(TTypeId id) const;
    [[nodiscard]] std::optional<std::string> getTypeByPinId(TPinId id) const;
    bool addBlockType(TBlockTypeId id, const std::string& block_type);
    [[nodiscard]] std::optional<std::string> getBlockType(TBlockTypeId id) const;
    bool addBlock(TBlockId id, const Block& block);
    [[nodiscard]] std::optional<Block> getBlock(TBlockId id) const;
    [[nodiscard]] std::optional<Block> getBlockByPinId(TPinId id) const;
    bool addLink(const Link& link);

private:
    std::map<TPinId, TBlockId> blockIdByPinId;
};

}

#endif //BLOCK_ENGINE_SCHEME_H
