//
// Created by Stepan on 25.09.2021.
//

#ifndef MODERN_CPP_DESIGN_CONNECTOR_H
#define MODERN_CPP_DESIGN_CONNECTOR_H

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
        return Connector::size();
    }

    Bus& getBus(size_t n) {
        return *Connector::at(n);
    }

    [[nodiscard]] const TBus& getBus(size_t n) const {
        return *Connector::at(n);
    }

    void setBus(size_t n, BusPtr bus) {
        Connector::at(n) = bus;
    }
};

}

#endif //MODERN_CPP_DESIGN_CONNECTOR_H
