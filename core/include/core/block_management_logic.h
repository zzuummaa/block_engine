//
// Created by Stepan on 03.01.2022.
//

#ifndef BLOCK_ENGINE_BLOCK_MANAGEMENT_LOGIC_H
#define BLOCK_ENGINE_BLOCK_MANAGEMENT_LOGIC_H

#include <memory>
#include <vector>

#include "bus.h"
#include "core/block/block.h"

namespace block_engine::core {

class BlockManagementLogic {
public:

    void setBlocks(std::map<int, BlockLogicBasePtr>&& blocks_) {
        this->blocks.swap(blocks_);
    }

    void setConnectors(std::map<int, std::pair<Connector, Connector>>&& connectors_) {
        this->connectors.swap(connectors_);
    }

    void connectBlocks() {
        if (connectors.size() != blocks.size()) throw std::invalid_argument(__PRETTY_FUNCTION__);

        auto con_it = connectors.begin();
        auto block_it = blocks.begin();

        for (; con_it != connectors.end(); con_it++, block_it++) {
            if (con_it->first != block_it->first) throw std::invalid_argument(__PRETTY_FUNCTION__);

            block_it->second->connectOutputs(Connector::output(con_it->second));
            block_it->second->connectInputs(Connector::input(con_it->second));
        }
    }

    BlockLogicBasePtr& getBlock(int id) {
        return blocks[id];
    }
private:
    std::map<int, BlockLogicBasePtr> blocks;
    std::map<int, std::pair<Connector, Connector>> connectors;

};

}

#endif //BLOCK_ENGINE_BLOCK_MANAGEMENT_LOGIC_H
