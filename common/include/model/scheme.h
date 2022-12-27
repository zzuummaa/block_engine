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
    std::map<TPinId, Pin> pins;
    std::set<Link> links;

    bool addType(TTypeId id, const std::string& type);
    [[nodiscard]] std::optional<std::string> getType(TTypeId id) const;
    [[nodiscard]] std::optional<TTypeId> getTypeId(const std::string& type) const;
    [[nodiscard]] std::optional<std::string> getTypeByPinId(TPinId id) const;

    bool addBlockType(TBlockTypeId id, const std::string& block_type);
    [[nodiscard]] std::optional<std::string> getBlockType(TBlockTypeId id) const;
    [[nodiscard]] std::optional<TBlockTypeId> getBlockTypeId(const std::string& blockType) const;

    bool addBlock(TBlockId id, const Block& block);
    [[nodiscard]] const std::map<TBlockTypeId, model::Block>& getBlocks() const;
    [[nodiscard]] std::optional<Block> getBlock(TBlockId id) const;
    [[nodiscard]] std::optional<Block> getBlockByPinId(TPinId id) const;

    bool addLink(const Link& link);

private:
    std::map<TTypeId, std::string> types;
    std::map<std::string, TTypeId> typeIds;

    std::map<TBlockTypeId, std::string> blockTypes;
    std::map<std::string, TBlockTypeId> blockTypeIds;

    std::map<TBlockId, Block> blocks;
    std::map<TPinId, TBlockId> blockIdByPinId;
};

}

#endif //BLOCK_ENGINE_SCHEME_H
