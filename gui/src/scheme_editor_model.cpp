#include "scheme_editor_model.h"

namespace block_engine {

SchemeEditorModel::SchemeEditorModel() {
}

void SchemeEditorModel::addBlock(TId id, const BlockTypeInfo& info) {
    if (!blocks.emplace(id, info).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
}

void SchemeEditorModel::addInput(TId blockId, TId inputId, const BusTypeInfo& info) {
    if (outputs.count(inputId)) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    if (!inputs.emplace(inputId, info).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    inputsByBlockId[blockId].emplace_back(inputId, info);
}

void SchemeEditorModel::addOutput(TId blockId, TId outputId, const BusTypeInfo& info) {
    if (inputs.count(outputId)) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    if (!outputs.emplace(outputId, info).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    outputsByBlockId[blockId].emplace_back(outputId, info);
}

bool SchemeEditorModel::addLink(TId fromPinId, TId toPinId) {
    if (fromPinId == toPinId) {
        return false;
    }

    if (!inputs.count(fromPinId) && !outputs.count(fromPinId)) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    if (!inputs.count(toPinId) && !outputs.count(toPinId)) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    TId inputId;
    TId outputId;

    if (inputs.count(fromPinId)) {
        inputId = fromPinId;

        if (outputs.count(toPinId)) {
            outputId = toPinId;
        } else {
            return false;
        }
    } else if (outputs.count(fromPinId)) {
        outputId = fromPinId;

        if (inputs.count(toPinId)) {
            inputId = toPinId;
        } else {
            return false;
        }
    }

    links.emplace(inputId, outputId);

    return true;
}

model::Scheme SchemeEditorModel::getScheme() const {
    model::Scheme scheme;

    model::TTypeId nextTypeId = 1;
    model::TBlockTypeId nextBlockTypeId = 1;
    model::TBlockId nextBlockId = 1;

    std::map<TId, model::TPinId> modelPinIdByPinId;

    for (const auto& [blockId, blockInfo] : blocks) {
        model::TBlockTypeId blockTypeId;

        auto blockTypeIdOpt = scheme.getBlockTypeId(blockInfo.name.toStdString());
        if (blockTypeIdOpt) {
            blockTypeId = *blockTypeIdOpt;
        } else {
            if (!scheme.addBlockType(nextBlockTypeId, blockInfo.name.toStdString())) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
            blockTypeId = nextBlockTypeId;
            nextBlockTypeId++;
        }

        auto block = model::Block {
            .id = nextBlockId,
            .block_type_id = blockTypeId
        };

        auto inputsIt = inputsByBlockId.find(blockId);
        auto outputsIt = outputsByBlockId.find(blockId);

        const static auto emptyPins = std::vector<std::pair<TId, BusTypeInfo>>();

        int inputIdx = 0;
        const auto& inputPairs = inputsIt != inputsByBlockId.end() ? inputsIt->second : emptyPins;
        for (const auto& [pinId, typeInfo] : inputPairs) {
            model::TTypeId typeId;

            auto typeIdOpt = scheme.getTypeId(typeInfo.name.toStdString());
            if (typeIdOpt) {
                typeId = *typeIdOpt;
            } else {
                if (!scheme.addType(nextTypeId, typeInfo.name.toStdString())) {
                    throw std::runtime_error(__PRETTY_FUNCTION__);
                }
                typeId = nextTypeId;
                nextTypeId++;
            }

            auto pin = model::Pin {
                .id = model::TPinId(block.id, true, inputIdx++),
                .typeId = typeId
            };
            block.inputs.push_back(pin);

            if (!modelPinIdByPinId.emplace(pinId, pin.id).second) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
        }

        int outputIdx = 0;
        const auto& outputPairs = outputsIt != outputsByBlockId.end() ? outputsIt->second : emptyPins;
        for (const auto& [pinId, typeInfo] : outputPairs) {
            model::TTypeId typeId;

            auto typeIdOpt = scheme.getTypeId(typeInfo.name.toStdString());
            if (typeIdOpt) {
                typeId = *typeIdOpt;
            } else {
                if (!scheme.addType(nextTypeId, typeInfo.name.toStdString())) {
                    throw std::runtime_error(__PRETTY_FUNCTION__);
                }
                typeId = nextTypeId;
                nextTypeId++;
            }

            auto pin = model::Pin {
                .id = model::TPinId(block.id, false, outputIdx++),
                .typeId = typeId
            };
            block.outputs.push_back(pin);

            if (!modelPinIdByPinId.emplace(pinId, pin.id).second) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
        }

        if (!scheme.addBlock(nextBlockId++, block)) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }
    }

    for (const auto& link : links) {
        auto outputIdIt = modelPinIdByPinId.find(link.first);
        auto inputIdIt = modelPinIdByPinId.find(link.second);

        if (outputIdIt == modelPinIdByPinId.end()
         || inputIdIt == modelPinIdByPinId.end()) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        if (!scheme.addLink(model::Link { .output = outputIdIt->second, .input = inputIdIt->second })) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }
    }

    return scheme;
}

}
