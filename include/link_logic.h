//
// Created by Stepan on 26.09.2021.
//

#ifndef BLOCK_ENGINE_LINK_LOGIC_H
#define BLOCK_ENGINE_LINK_LOGIC_H

#include <map>
#include <memory>
#include "../core/include/core/type_info.h"
#include "../core/include/core/block.h"
#include "storage.h"

class LinkLogic {
public:
    typedef std::shared_ptr<IBlockLogic> IBlockPtr;

    void addBlock(int block_id, IBlockPtr block, int inputs_count, int outputs_count);
    void addLink(int block_out_id, int bus_out_idx, int block_in_id, int bus_in_idx, const TypeInfo& info);
    void linkBlocks();

private:
    struct BlockInfo {
        IBlockPtr block;
        Connector input;
        Connector output;
    };

    std::map<int, BlockInfo> blocks;
    DataStorage storage;
};

#endif //BLOCK_ENGINE_LINK_LOGIC_H
