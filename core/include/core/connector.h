//
// Created by Stepan on 25.09.2021.
//

#ifndef MODERN_CPP_DESIGN_CONNECTOR_H
#define MODERN_CPP_DESIGN_CONNECTOR_H

#include <utility>
#include <vector>
#include "bus.h"

namespace block_engine::core {

struct Connector : private std::vector<BusPtr> {
    typedef typename BusPtr::element_type TBus;

    static const Connector& input(const std::pair<Connector, Connector>& pair) {
        return pair.first;
    }

    static Connector& input(std::pair<Connector, Connector>& pair) {
        return pair.first;
    }

    static const Connector& output(const std::pair<Connector, Connector>& pair) {
        return pair.second;
    }

    static Connector& output(std::pair<Connector, Connector>& pair) {
        return pair.second;
    }

    Connector() = default;
    Connector(Connector&& other) = default;

    explicit Connector(unsigned long long int count) : std::vector<BusPtr>(count) {}

    Connector& operator=(const Connector& connector) = default;
    Connector& operator=(Connector&& connector) noexcept = default;

    [[nodiscard]] size_t count() const {
        return size();
    }

    Bus& getBus(size_t n) {
        return *at(n);
    }

    [[nodiscard]] const TBus& getBus(size_t n) const {
        return *at(n);
    }

    void setBusIfEmptyOrError(size_t n, BusPtr bus) {
        if (at(n)) throw std::invalid_argument(__PRETTY_FUNCTION__);
        at(n) = std::move(bus);
    }
};

}

#endif //MODERN_CPP_DESIGN_CONNECTOR_H
