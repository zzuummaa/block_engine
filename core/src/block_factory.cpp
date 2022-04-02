#include "core/block_factory.h"

#include "core/blocks/console_block.h"
#include "core/blocks/const_block.h"
#include "core/blocks/sum_block.h"
#include "core/blocks/limit_block.h"

using namespace block_engine::core;
using namespace block_engine::core::blocks;

BlockFactory::TBlock BlockFactory::createBlockByName(const std::string &name) {
    auto it = map.find(name);
    return it != map.end() ? it->second() : throw std::runtime_error(__PRETTY_FUNCTION__);
}

BlockFactory::BlockFactory(const BlockFactory::TBlockFactoryMap &map) : map(map) {}

template <typename TBlock, typename ...TArgs>
auto make_block_initializer(const std::string& block_name, TArgs ...args) {
    return std::make_pair(block_name, [=](){ return new TBlock(args...); });
}

BlockFactory block_engine::core::make_block_factory() {
    BlockFactory::TBlockFactoryMap factory = {
        make_block_initializer<ConsoleBlockLogic<int>>("console_int"),
        make_block_initializer<ConstBlockLogic<int>>("const_int", 0),
        make_block_initializer<SumBlockLogic<int>>("sum_2_int"),
        make_block_initializer<LimitBlockLogic<int>>("limit_int"),
    };

    return BlockFactory{factory};
}
