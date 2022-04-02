//
// Created by Stepan on 26.09.2021.
//

#include "link_logic.h"
#include <stdexcept>

using namespace block_engine::core;

void LinkLogic::addBlock(int block_id, IBlockPtr block, int inputs_count, int outputs_count) {
    auto it = blocks.find(block_id);
    if (it != blocks.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
    blocks[block_id] = BlockInfo{
        .block = std::move(block),
        .input = Connector(inputs_count),
        .output = Connector(outputs_count)
    };
}

void LinkLogic::addLink(
    int block_out_id, int bus_out_idx,
    int block_in_id, int bus_in_idx,
    const TypeInfo& info) {

    auto in_block_it = blocks.find(block_in_id);
    if (in_block_it == blocks.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
    auto& in_connector = in_block_it->second.input;

    auto out_block_it = blocks.find(block_out_id);
    if (out_block_it == blocks.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
    auto& out_connector = out_block_it->second.output;

//    auto dataRef = storage.alloc(info);
//
//    in_connector.connect(bus_in_idx, Bus(dataRef.data()));
//    out_connector.connect(src_bus_idx, Bus(dataRef.data()));
}

void LinkLogic::linkBlocks() {
    for (auto& [key, block_info]: blocks) {
        block_info.block->connectInputs(block_info.input);
        block_info.block->connectOutputs(block_info.output);
    }
}
