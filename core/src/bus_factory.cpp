#include "core/bus_factory.h"

using namespace block_engine::core;

BusFactory::BusFactory(BusFactory::TBusFactoryMap map) : map(std::move(map)) {}

Bus BusFactory::createBusByName(const std::string &name) {
    auto it = map.find(name);
    return it != map.end() ? it->second() : throw std::runtime_error(__PRETTY_FUNCTION__);
}

BusPtr BusFactory::createBusPtrByName(const std::string &name) {
    auto it = map.find(name);
    return it != map.end() ? makeBusPtr(std::move(it->second())) : BusPtr();
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
