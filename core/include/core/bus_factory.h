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
    typedef std::map<std::string, std::function<Bus()>> TBusFactoryMap;

    explicit BusFactory(TBusFactoryMap map);

    Bus createBusByName(const std::string& name);

    BusPtr createBusPtrByName(const std::string& name);

private:
    TBusFactoryMap map;
};

BusFactory make_bus_factory();
}

#endif //BLOCK_ENGINE_BUS_FACTORY_H
