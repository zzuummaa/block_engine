#pragma once

#include <map>
#include <string>
#include <functional>

#include <pin_group_holder.h>
#include <qblock.h>

struct BlockInitializer {
    friend struct BlockFactory;

    using TBlock = QBlock*;
    using TPinCollection = PinGroupHolder;

    TBlock makeBlock() const;

    std::function<TBlock(const BlockInitializer&)> blockInitializer;
    std::function<TPinCollection()> inputsInitializer;
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
