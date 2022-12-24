#include "block_factory.h"

#include <block/block_description.h>
#include <bus_types.h>

using namespace block_engine;
using namespace block_engine::block;

BlockInitializer::TBlock BlockInitializer::makeBlock() const {
    return blockInitializer(*this);
}

BlockFactory::BlockFactory(const BlockFactory::TBlockFactoryMap& map) : map(map) {}

BlockFactory::BlockFactory(BlockFactory::TBlockFactoryMap&& map) : map(std::move(map)) {}

const BlockFactory::TBlockInitializer& BlockFactory::getInitializerByBlockName(const QString& block_name) {
    const auto it = map.find({block_name});
    return it != map.end() ? it->second : throw std::runtime_error(__PRETTY_FUNCTION__);
}

BlockInitializer::TBlock BlockFactory::createBlockByName(const QString& block_name) {
    auto it = map.find({block_name});
    return it != map.end() ? it->second.makeBlock() : throw std::runtime_error(__PRETTY_FUNCTION__);
}

//template<typename = void, typename>
//void createPins(BusGroupHolder& pins, bool) {
//
//}

template<typename TPin, typename, std::enable_if_t<!std::is_base_of_v<Marker, TPin>, int> = 0>
void createPins(PinGroupHolder& pins, bool isOptional, bool isInput) {
    static_assert(!std::is_base_of_v<Marker, TPin>, "Invalid function for marker");

    const auto& name = BusType<TPin>().name;
    pins = {new QPin({name}, isInput), isOptional};
}

template<typename TPin, typename, std::enable_if_t<std::is_same_v<Empty, TPin>, int> = 0>
void createPins(PinGroupHolder&, bool, bool) {
}

template<typename TPin, typename TInstance, std::enable_if_t<std::is_same_v<Instance, TPin>, int> = 0>
void createPins(PinGroupHolder& pins, bool isOptional, bool isInput) {
    const auto& name = BusType<TInstance>().name;
    pins = {new QPin({name}, isInput), isOptional};
}

template<typename TPins, typename TInstance, std::enable_if_t<IsRange<TPins, Range>::value, int> = 0>
void createPins(PinGroupHolder& pins, bool isOptional, bool isInput) {
    pins = PinGroupHolder(PinGroupHolder::THolderCollection(), isOptional);
    auto& subHolders = pins.holderCollection();

    for (size_t i = 0; i < TPins::end; i++) {
        auto subHolder = std::make_unique<PinGroupHolder>();
        createPins<typename TPins::TPin, TInstance>(*subHolder, i < TPins::begin, isInput);
        if (!subHolder->isEmpty()) {
            subHolders.push_back(std::move(subHolder));
        }
    }
}

template<typename TPinAccessor>
auto createPins() {
    PinGroupHolder pins;
    createPins<typename TPinAccessor::TPins, typename TPinAccessor::TInstance>(pins, false, TPinAccessor::isInput);
    return pins;
}

template<typename TBlock, typename TDescription, typename TInstance, typename ...TArgs>
auto makeBlockInitializer(const QString& block_name, TArgs ...args) {
    const auto info = BlockTypeInfo{block_name};
    return std::make_pair(
        info,
        BlockInitializer{
            [=](const auto& initializer) {
                return new TBlock(info, initializer.inputsInitializer(), args...);
            },
            [=]() {
                return createPins<InputAccessor<TDescription, TInstance>>();
            }
        });
}

struct BlockFactoryBuilder {
public:
    template<typename TBlock, typename TDescription, typename ...TArgs>
    void create_block_initializers(TArgs ...args) {
        TDescription description;

        auto type_visitor = [&](auto&& type_val) {
            using TInstance = typename std::decay<decltype(type_val)>::type;
            const auto name = QString(description.name) + '_' + BusType<TInstance>().name;
            factory_map.insert(makeBlockInitializer<TBlock, TDescription, TInstance>(name, args...));
        };

        std::apply(
            [&type_visitor](auto&& ... args) { (type_visitor(args), ...); },
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

//    using TAccessor = InputAccessor<SumBlockDescription, Nullable<double>>;
//    std::cout << (TAccessor::hasPins ? "true" : "false") << std::endl;
//    std::cout << typeid(TAccessor::TPins).name() << std::endl;
//
//    BusGroupHolder holder;
//    createPins<TAccessor::TPins, TAccessor::TInstance>(holder, false);

    return builder.build();
}
