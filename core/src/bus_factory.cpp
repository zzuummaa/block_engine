#include "core/bus_factory.h"

using namespace block_engine::core;

BusFactory::BusFactory(const BusFactory::TBusFactoryMap &map) : map(map) {}

BusFactory::TBus BusFactory::createBusByName(const std::string &name) {
    auto it = map.find(name);
    return it != map.end() ? it->second() : BusFactory::TBus();
}

template <typename TData>
auto make_bus_initializer(const std::string& type_name) {
    return std::make_pair(type_name, [](){ return Bus(TData()); });
}

BusFactory block_engine::core::make_bus_factory() {
    BusFactory::TBusFactoryMap factory = {
        make_bus_initializer<int>("int"),
        make_bus_initializer<double>("double")
    };

    return BusFactory{factory};
}
