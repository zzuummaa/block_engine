#ifndef BLOCK_ENGINE_BLOCK_FACTORY_H
#define BLOCK_ENGINE_BLOCK_FACTORY_H

#include <map>
#include <string>
#include <functional>

#include "gui/block_type_info.h"
#include "qblock.h"

namespace block_engine::gui {

struct BlockFactory {
public:
    using TBlock = QBlock*;
    using TBlockFactoryMap = std::map<BlockTypeInfo, std::function<TBlock()>>;

    TBlockFactoryMap map;

    explicit BlockFactory(const TBlockFactoryMap &map);

    TBlock createBlockByName(const BlockTypeInfo&);
};

BlockFactory make_block_factory();

}

#endif //BLOCK_ENGINE_BLOCK_FACTORY_H
