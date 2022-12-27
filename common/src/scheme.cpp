//
// Created by Stepan on 02.01.2022.
//

#include <algorithm>
#include <stdexcept>

#include "model/scheme.h"

namespace block_engine::model {

bool Scheme::addType(TTypeId id, const std::string& type) {
    if (typeIds.count(type) > 0
     || types.count(id) > 0) {
        return false;
    }

    if (!types.emplace(id, type).second
        || !typeIds.emplace(type, id).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    return true;
}

std::optional<std::string> Scheme::getType(TTypeId id) const {
    auto it = types.find(id);
    return it != types.end() ? std::make_optional(it->second) : std::nullopt;
}

std::optional<TTypeId> Scheme::getTypeId(const std::string& type) const {
    auto it = typeIds.find(type);
    return it != typeIds.end() ? std::make_optional(it->second) : std::nullopt;
}

std::optional<std::string> Scheme::getTypeByPinId(TPinId id) const {
    auto it = pins.find(id);
    return it != pins.end() ? getType(it->second.typeId) : std::nullopt;
}

bool Scheme::addBlockType(TBlockTypeId id, const std::string& blockType) {
    if (blockTypeIds.count(blockType) > 0
     || blockTypes.count(id) > 0) {
        return false;
    }

    if (!blockTypes.emplace(id, blockType).second
     || !blockTypeIds.emplace(blockType, id).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    return true;
}

std::optional<std::string> Scheme::getBlockType(TBlockTypeId id) const {
    auto it = blockTypes.find(id);
    return it != blockTypes.end() ? std::make_optional(it->second) : std::nullopt;
}

std::optional<TBlockTypeId> Scheme::getBlockTypeId(const std::string& blockType) const {
    auto it = blockTypeIds.find(blockType);
    return it != blockTypeIds.end() ? std::make_optional(it->second) : std::nullopt;
}

bool Scheme::addBlock(TBlockId id, const Block &block) {
    auto countPins = [&](const auto& pin){
        return blockIdByPinId.count(pin.id) > 0 || pins.count(pin.id);
    };

    if (blocks.count(block.id) > 0
    || std::any_of(block.inputs.begin(), block.inputs.end(), countPins)
    || std::any_of(block.outputs.begin(), block.outputs.end(), countPins)) {
        return false;
    }

    auto emplacePin = [&](const auto& pin){
        blockIdByPinId.emplace(pin.id, block.id);
        pins.emplace(pin.id, pin);
    };

    std::for_each(block.inputs.begin(), block.inputs.end(), emplacePin);
    std::for_each(block.outputs.begin(), block.outputs.end(), emplacePin);

    blocks.emplace(id, block);

    return true;
}

const std::map<TBlockTypeId, model::Block>& Scheme::getBlocks() const {
    return blocks;
}

std::optional<Block> Scheme::getBlock(TBlockId id) const {
    auto it = blocks.find(id);
    return it != blocks.end() ? std::make_optional(it->second) : std::nullopt;
}

std::optional<Block> Scheme::getBlockByPinId(TPinId id) const {
    return getBlock(id.getBlockId());
}

bool Scheme::addLink(const Link& link) {
    if (blockIdByPinId.count(link.output) == 0
    ||  blockIdByPinId.count(link.input) == 0) {
        return false;
    }

    return links.emplace(link).second;
}

}