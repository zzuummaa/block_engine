#include "block_factory.h"

BlockFactory::BlockFactory(const BlockFactory::TBlockFactoryMap& map) : map(map) {}

const BlockFactory::TBlockInitializer& BlockFactory::getInitializerByBlockName(const QString& block_name) {
    const auto it = map.find({block_name});
    return it != map.end() ? it->second : throw std::runtime_error(__PRETTY_FUNCTION__);
}

template<typename TBlock, typename ...TArgs>
auto make_block_initializer(const QString& block_name, TArgs ...args) {
    const auto info = BlockTypeInfo{block_name};
    return std::make_pair(info, [=]() { return new TBlock(info, args...); });
}

BlockFactory::TBlock BlockFactory::createBlockByName(const QString& block_name) {
    auto it = map.find({block_name});
    return it != map.end() ? it->second() : throw std::runtime_error(__PRETTY_FUNCTION__);
}

BlockFactory make_block_factory() {
    BlockFactory::TBlockFactoryMap factory = {
        make_block_initializer<QBlock>("console_int"),
        make_block_initializer<QBlock>("const_int"),
        make_block_initializer<QBlock>("sum_2_int"),
        make_block_initializer<QBlock>("limit_int"),
    };

    return BlockFactory{factory};
}
