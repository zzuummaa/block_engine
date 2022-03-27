//
// Created by Stepan on 26.03.2022.
//

#ifndef BLOCK_ENGINE_BUS_FACTORY_H
#define BLOCK_ENGINE_BUS_FACTORY_H

#include <map>
#include <functional>
#include "bus.h"

namespace block_engine::core {

class BusFactory {
public:
    using TBus = Bus;
    using TBusFactoryMap = std::map<std::string, std::function<Bus()>>;;

    explicit BusFactory(const TBusFactoryMap &map);

    TBus createBusByName(const std::string& name);

private:
    TBusFactoryMap map;
};

BusFactory make_bus_factory();
}

#endif //BLOCK_ENGINE_BUS_FACTORY_H
