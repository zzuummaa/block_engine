//
// Created by Stepan on 11.12.2022.
//

#include <pin_group_holder.h>

PinGroupHolder::TBus PinGroupHolder::bus() {
    return get<PinGroupHolder::TBus>();
}

PinGroupHolder::TBusCollection& PinGroupHolder::busCollection() {
    return get<PinGroupHolder::TBusCollection>();
}

PinGroupHolder::THolderCollection& PinGroupHolder::holderCollection() {
    return get<PinGroupHolder::THolderCollection>();
}

