//
// Created by Stepan on 02.01.2022.
//

#include "model/scheme.h"

namespace model {

    bool Scheme::addType(int type_id, const std::string &type) {
        return types.emplace(type_id, type).second;
    }

    std::optional<std::string> Scheme::getType(int type_id) {
        auto it = types.find(type_id);
        return it != types.end() ? std::make_optional(it->second) : std::nullopt;
    }

    bool Scheme::addBlockType(int block_id, const std::string &block_type) {
        return block_types.emplace(block_id, block_type).second;
    }

    std::optional<std::string> Scheme::getBlockType(int block_type_id) {
        auto it = block_types.find(block_type_id);
        return it != block_types.end() ? std::make_optional(it->second) : std::nullopt;
    }

    bool Scheme::addBlock(int block_id, const Block &block) {
        return blocks.emplace(block_id, block).second;
    }

    std::optional<Block> Scheme::getBlock(int block_id) {
        auto it = blocks.find(block_id);
        return it != blocks.end() ? std::make_optional(it->second) : std::nullopt;
    }

    bool Scheme::addLink(const Link &link) {
        return links.emplace(link).second;
    }
}