//
// Created by Stepan on 27.03.2022.
//

#ifndef BLOCK_ENGINE_BLOCK_FACTORY_H
#define BLOCK_ENGINE_BLOCK_FACTORY_H

#include <map>
#include <functional>
#include <memory>
#include "block.h"

namespace block_engine::core {

class BlockFactory {
public:
    using TBlock = IBlockLogic*;
    using TBlockFactoryMap = std::map<std::string, std::function<BlockFactory::TBlock()>>;

    explicit BlockFactory(const TBlockFactoryMap &map);

    TBlock createBlockByName(const std::string& name);

private:
    TBlockFactoryMap map;
};

BlockFactory make_block_factory();

}

#endif //BLOCK_ENGINE_BLOCK_FACTORY_H
