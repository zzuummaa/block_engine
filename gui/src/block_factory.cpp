#include "block_factory.h"

#include <block/block_description.h>
#include "bus_types.h"

using namespace block_engine;
using namespace block_engine::block;

BlockFactory::BlockFactory(const BlockFactory::TBlockFactoryMap& map) : map(map) {}

BlockFactory::BlockFactory(BlockFactory::TBlockFactoryMap&& map) : map(std::move(map)) {}

const BlockFactory::TBlockInitializer& BlockFactory::getInitializerByBlockName(const QString& block_name) {
    const auto it = map.find({block_name});
    return it != map.end() ? it->second : throw std::runtime_error(__PRETTY_FUNCTION__);
}

BlockFactory::TBlock BlockFactory::createBlockByName(const QString& block_name) {
    auto it = map.find({block_name});
    return it != map.end() ? it->second() : throw std::runtime_error(__PRETTY_FUNCTION__);
}

template<typename TBlock, typename ...TArgs>
auto make_block_initializer(const QString& block_name, TArgs ...args) {
    const auto info = BlockTypeInfo{block_name};
    return std::make_pair(info, [=]() { return new TBlock(info, args...); });
}

struct BlockFactoryBuilder {
public:
    template<typename TBlock, typename TDescription, typename ...TArgs>
    void create_block_initializers(TArgs ...args) {
        TDescription description;

        auto type_visitor = [&](auto&& type_val) {
            using TType = typename std::decay<decltype(type_val)>::type;
            const auto name = QString(description.name) + '_' + BusType<TType>().name;
            factory_map.insert(make_block_initializer<TBlock>(name, args...));
        };

        std::apply([&type_visitor](auto&& ... args) { (type_visitor(args), ...); },
                   typename TDescription::TInstances());
    }

    BlockFactory build() {
        return BlockFactory(std::move(factory_map));
    }

private:
    BlockFactory::TBlockFactoryMap factory_map;
};

BlockFactory make_block_factory() {
    BlockFactoryBuilder builder;

    builder.create_block_initializers<QBlock, ConsoleBlockDescription>();
    builder.create_block_initializers<QBlock, ConstBlockDescription>();
    builder.create_block_initializers<QBlock, SumBlockDescription>();
    builder.create_block_initializers<QBlock, LimitBlockDescription>();

    return builder.build();
}
