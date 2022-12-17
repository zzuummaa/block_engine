#pragma once

#include <block_type_info.h>
#include <bus_type_info.h>

#include <map>
#include <set>

class SchemeEditorModel {
public:
    using TId = std::uintptr_t;

    void addBlock(TId id, const BlockTypeInfo&);
    void addInput(TId blockId, TId inputId, const BusTypeInfo&);
    void addOutput(TId blockId, TId outputId, const BusTypeInfo&);
    bool addLink(TId fromPinId, TId toPinId);


private:
    std::map<TId, BlockTypeInfo> blocks;
    std::map<TId, BusTypeInfo> inputs;
    std::map<TId, BusTypeInfo> outputs;

    std::set<std::pair<TId, TId>> links;
};