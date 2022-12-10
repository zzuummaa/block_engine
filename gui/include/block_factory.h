#ifndef BLOCK_ENGINE_BLOCK_FACTORY_H
#define BLOCK_ENGINE_BLOCK_FACTORY_H

#include <map>
#include <string>
#include <functional>

#include "block_type_info.h"
#include "qblock.h"

struct BlockFactory {
public:
    using TBlock = QBlock*;
    using TBlockInitializer = std::function<TBlock()>;
    using TBlockFactoryMap = std::map<BlockTypeInfo, TBlockInitializer>;

    TBlockFactoryMap map;

    BlockFactory() = default;
    explicit BlockFactory(const TBlockFactoryMap &map);

    const TBlockInitializer& getInitializerByBlockName(const QString& block_name);

    TBlock createBlockByName(const QString& block_name);
};

BlockFactory make_block_factory();


#endif //BLOCK_ENGINE_BLOCK_FACTORY_H
