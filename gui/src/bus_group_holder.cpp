//
// Created by Stepan on 11.12.2022.
//

#include <bus_group_holder.h>

BusGroupHolder::TBus BusGroupHolder::bus() {
    return get<BusGroupHolder::TBus>();
}

BusGroupHolder::TBusCollection& BusGroupHolder::busCollection() {
    return get<BusGroupHolder::TBusCollection>();
}

BusGroupHolder::THolderCollection& BusGroupHolder::holderCollection() {
    return get<BusGroupHolder::THolderCollection>();
}

