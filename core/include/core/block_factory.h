//
// Created by Stepan on 27.03.2022.
//

#ifndef BLOCK_ENGINE_BLOCK_FACTORY_H
#define BLOCK_ENGINE_BLOCK_FACTORY_H

#include <map>
#include <functional>
#include <memory>
#include "core/block/block.h"

namespace block_engine::core {

class BlockFactoryBuilder;

class BlockFactory {
    friend class BlockFactoryBuilder;
public:
    using TBlock = BlockLogicBasePtr;
    using TBlockFactoryMap = std::map<std::string, std::function<TBlock()>>;

    explicit BlockFactory(const TBlockFactoryMap& map);
    explicit BlockFactory(TBlockFactoryMap&& map);

    TBlock createBlockByName(const std::string& name);

private:
    TBlockFactoryMap map;
};

BlockFactory makeBlockFactory();

}

#endif //BLOCK_ENGINE_BLOCK_FACTORY_H
