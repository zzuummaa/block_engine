#include "scheme_editor_model.h"

SchemeEditorModel::SchemeEditorModel()
//    : nextTypeId(1)
//    , nextBlockTypeId(1)
//    , nextBlockId(1)
{}

void SchemeEditorModel::addBlock(TId id, const BlockTypeInfo& info) {
    if (!blocks.emplace(id, info).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
}

void SchemeEditorModel::addInput(TId /*blockId*/, TId inputId, const BusTypeInfo& info) {
    if (outputs.count(inputId)) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    if (!inputs.emplace(inputId, info).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
}

void SchemeEditorModel::addOutput(TId /*blockId*/, TId outputId, const BusTypeInfo& info) {
    if (inputs.count(outputId)) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }

    if (!outputs.emplace(outputId, info).second) {
        throw std::runtime_error(__PRETTY_FUNCTION__);
    }
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

