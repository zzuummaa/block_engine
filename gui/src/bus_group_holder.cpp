//
// Created by Stepan on 11.12.2022.
//

#include <bus_group_holder.h>

bool BusGroupHolder::isBus() const {
    return std::holds_alternative<BusGroupHolder::TBus>(value);
}

bool BusGroupHolder::isBusCollection() const {
    return std::holds_alternative<BusGroupHolder::TBusCollection>(value);
}

bool BusGroupHolder::isHolderCollection() const {
    return std::holds_alternative<BusGroupHolder::THolderCollection>(value);
}

BusGroupHolder::TBus BusGroupHolder::bus() {
    return get<BusGroupHolder::TBus>();
}

BusGroupHolder::TBusCollection& BusGroupHolder::busCollection() {
    return get<BusGroupHolder::TBusCollection>();
}

BusGroupHolder::THolderCollection& BusGroupHolder::holderCollection() {
    return get<BusGroupHolder::THolderCollection>();
}

BusGroupHolder& BusGroupHolder::operator=(BusGroupHolder other) {
    value = std::move(other.value);
    std::swap(isOptional, other.isOptional);
    return *this;
}
