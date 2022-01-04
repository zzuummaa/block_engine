//
// Created by Stepan on 03.01.2022.
//

#ifndef BLOCK_ENGINE_BLOCK_MANAGEMENT_LOGIC_H
#define BLOCK_ENGINE_BLOCK_MANAGEMENT_LOGIC_H

#include <memory>
#include <vector>

#include "bus.h"
#include "block.h"

class BlockManagementLogic {
public:
    typedef IBlock* IBlockPtr;

    ~BlockManagementLogic() {

    }

    void setBlocks(std::map<int, IBlockPtr>&& blocks_) {
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

    IBlockPtr& getBlock(int id) {
        return blocks[id];
    }
private:
    std::map<int, std::pair<Connector, Connector>> connectors;
    std::map<int, IBlockPtr> blocks;

};

#endif //BLOCK_ENGINE_BLOCK_MANAGEMENT_LOGIC_H
