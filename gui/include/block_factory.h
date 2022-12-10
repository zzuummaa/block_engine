#pragma once

#include <map>
#include <string>
#include <functional>

#include "qblock.h"

struct BlockFactory {
public:
    using TBlock = QBlock*;
    using TBlockInitializer = std::function<TBlock()>;
    using TBlockFactoryMap = std::map<BlockTypeInfo, TBlockInitializer>;

    TBlockFactoryMap map;

    BlockFactory() = default;

    explicit BlockFactory(const TBlockFactoryMap& map);

    explicit BlockFactory(TBlockFactoryMap&& map);

    const TBlockInitializer& getInitializerByBlockName(const QString& block_name);

    TBlock createBlockByName(const QString& block_name);
};

BlockFactory make_block_factory();
