//
// Created by Stepan on 25.09.2021.
//

#ifndef MODERN_CPP_DESIGN_CONNECTOR_H
#define MODERN_CPP_DESIGN_CONNECTOR_H

#include <utility>
#include <vector>
#include "bus.h"

namespace block_engine::core {

struct Connector;

using ConnectorPair = std::pair<Connector, Connector>;

struct Connector : private std::vector<BusPtr> {
    using TBus = BusPtr::element_type;

    static const Connector& input(const ConnectorPair& pair) {
        return pair.first;
    }

    static Connector& input(ConnectorPair& pair) {
        return pair.first;
    }

    static const Connector& output(const ConnectorPair& pair) {
        return pair.second;
    }

    static Connector& output(ConnectorPair& pair) {
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

    void setBus(size_t n, BusPtr bus) {
        if (at(n)) throw std::invalid_argument(__PRETTY_FUNCTION__);
        at(n) = std::move(bus);
    }

    bool isLinked() const {
        return std::all_of(begin(), end(), [](const auto& bus){ return bus.get() != nullptr; });
    }
};

}

#endif //MODERN_CPP_DESIGN_CONNECTOR_H
