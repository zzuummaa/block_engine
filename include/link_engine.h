//
// Created by Stepan on 26.09.2021.
//

#ifndef BLOCK_ENGINE_LINK_ENGINE_H
#define BLOCK_ENGINE_LINK_ENGINE_H

#include <map>
#include "type_info.h"
#include "block.h"
#include "storage.h"

class LinkEngine {
    struct BlockInfo {
        IBlock* block = nullptr;
        Connector input;
        Connector output;
    };

    std::map<int, BlockInfo> blocks;
    DataStorage storage;
public:
    void addBlock(int block_id, IBlock* block, int inputs_count, int outputs_count);
    void addLink(int block_out_id, int bus_out_idx, int block_in_id, int bus_in_idx, const TypeInfo& info);
    void linkBlocks();
};

#endif //BLOCK_ENGINE_LINK_ENGINE_H
