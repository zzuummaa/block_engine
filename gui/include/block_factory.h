#pragma once

#include <map>
#include <string>
#include <functional>

#include <bus_group_holder.h>
#include <qblock.h>

struct BlockInitializer {
    using TBlock = QBlock*;
    using TBusCollection = std::vector<BusGroupHolder>;

    std::function<TBlock()> makeBlock;
    std::function<TBusCollection()> makeInputs;
};

struct BlockFactory {
public:

    using TBlockInitializer = BlockInitializer;
    using TBlockFactoryMap = std::map<BlockTypeInfo, TBlockInitializer>;

    TBlockFactoryMap map;

    BlockFactory() = default;

    explicit BlockFactory(const TBlockFactoryMap& map);

    explicit BlockFactory(TBlockFactoryMap&& map);

    const TBlockInitializer& getInitializerByBlockName(const QString& block_name);

    BlockInitializer::TBlock createBlockByName(const QString& block_name);
};

BlockFactory make_block_factory();
