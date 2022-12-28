#pragma once

#include "block_type_info.h"
#include "bus_type_info.h"

#include <map>
#include <set>

#include "model/scheme.h"

class SchemeEditorModel {
public:
    using TId = std::uintptr_t;

    SchemeEditorModel();

    void addBlock(TId id, const BlockTypeInfo&);

    void addInput(TId blockId, TId inputId, const BusTypeInfo&);

    void addOutput(TId blockId, TId outputId, const BusTypeInfo&);

    bool addLink(TId fromPinId, TId toPinId);

    [[nodiscard]] block_engine::model::Scheme getScheme() const;
private:

    std::map<TId, BlockTypeInfo> blocks;

    std::map<TId, std::vector<std::pair<TId, BusTypeInfo>>> inputsByBlockId;
    std::map<TId, std::vector<std::pair<TId, BusTypeInfo>>> outputsByBlockId;

    std::map<TId, BusTypeInfo> inputs;
    std::map<TId, BusTypeInfo> outputs;

    std::set<std::pair<TId, TId>> links;
};
