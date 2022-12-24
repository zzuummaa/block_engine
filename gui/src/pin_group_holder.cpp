//
// Created by Stepan on 11.12.2022.
//

#include <pin_group_holder.h>

PinGroupHolder::TPin PinGroupHolder::bus() {
    return get<PinGroupHolder::TPin>();
}

PinGroupHolder::TPinCollection& PinGroupHolder::busCollection() {
    return get<PinGroupHolder::TPinCollection>();
}

PinGroupHolder::THolderCollection& PinGroupHolder::holderCollection() {
    return get<PinGroupHolder::THolderCollection>();
}

