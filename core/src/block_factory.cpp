#include "core/block_factory.h"

#include <utility>

#include "bus_types.h"
#include <block/block_description.h>

#include "core/block/console_block.h"
#include "core/block/const_block.h"
#include "core/block/sum_block.h"
#include "core/block/limit_block.h"

using namespace block_engine::block;
using namespace block_engine::core::block;

namespace block_engine::core {

BlockFactory::BlockFactory(const BlockFactory::TBlockFactoryMap& map) : map(map) {}

BlockFactory::BlockFactory(BlockFactory::TBlockFactoryMap&& map) : map(std::move(map)) {}

BlockFactory::TBlock BlockFactory::createBlockByName(const std::string& name) {
    auto it = map.find(name);
    return it != map.end() ? it->second() : throw std::runtime_error(__PRETTY_FUNCTION__);
}

template<typename TBlock, typename ...TArgs>
auto make_block_initializer(const std::string& block_name, TArgs ...args) {
    return std::make_pair(block_name, [=]() { return MakeBlockLogicBasePtr<TBlock>(args...); });
}

struct BlockFactoryBuilder {
public:
    template<template<typename ...> typename TBlock, typename TDescription, typename ...TArgs>
    void create_block_initializers(TArgs ...args) {
        TDescription description;

        auto type_visitor = [&](auto&& type_val) {
            using TType = typename std::decay<decltype(type_val)>::type;
            const auto name = std::string(description.name) + '_' + BusType<TType>().name;
            factory_map.insert(make_block_initializer<TBlock<TType>>(name, args...));
        };

        std::apply([&type_visitor](auto&& ... args) { (type_visitor(args), ...); }, typename TDescription::TInstances());
    }

    BlockFactory build() {
        return BlockFactory(std::move(factory_map));
    }
private:
    BlockFactory::TBlockFactoryMap factory_map;
};

BlockFactory makeBlockFactory() {
    BlockFactoryBuilder builder;

    builder.create_block_initializers<ConsoleBlockLogic, ConsoleBlockDescription>();
    builder.create_block_initializers<ConstBlockLogic, ConstBlockDescription>(0);
    builder.create_block_initializers<SumBlockLogic, SumBlockDescription>();
    builder.create_block_initializers<LimitBlockLogic, LimitBlockDescription>();

    return builder.build();
}

}
