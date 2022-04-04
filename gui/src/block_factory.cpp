#include "gui/block_factory.h"

namespace block_engine::gui {

BlockFactory::TBlock BlockFactory::createBlockByName(const BlockTypeInfo& info) {
    auto it = map.find(info);
    return it != map.end() ? it->second() : throw std::runtime_error(__PRETTY_FUNCTION__);
}

BlockFactory::BlockFactory(const BlockFactory::TBlockFactoryMap& map) : map(map) {}

template<typename TBlock, typename ...TArgs>
auto make_block_initializer(const QString& block_name, TArgs ...args) {
    return std::make_pair(BlockTypeInfo{block_name}, [=]() { return new TBlock(args...); });
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

}