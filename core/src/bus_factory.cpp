#include "core/bus_factory.h"
#include "bus_types.h"

namespace block_engine::core {

BusFactory::BusFactory(BusFactory::TBusFactoryMap map) : map(std::move(map)) {}

Bus BusFactory::createBusByName(const std::string& name) {
    auto it = map.find(name);
    return it != map.end() ? it->second() : throw std::runtime_error(__PRETTY_FUNCTION__);
}

BusPtr BusFactory::createBusPtrByName(const std::string& name) {
    auto it = map.find(name);
    return it != map.end() ? makeBusPtr(std::move(it->second())) : BusPtr();
}

template<typename TData>
auto make_bus_initializer() {
    return std::make_pair(BusType<TData>().name, []() { return Bus(TData()); });
}

BusFactory makeBusFactory() {
    BusFactory::TBusFactoryMap factory = {
            make_bus_initializer<int>(),
            make_bus_initializer<double>()
    };

    return BusFactory{factory};
}

}