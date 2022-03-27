//
// Created by Stepan on 25.09.2021.
//

#ifndef MODERN_CPP_DESIGN_CONNECTOR_H
#define MODERN_CPP_DESIGN_CONNECTOR_H

#include <vector>
#include "bus.h"

struct Connector : private std::vector<Bus*> {
public:

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

    explicit Connector(unsigned long long int count) : vector(count) {}

    Connector& operator=(const Connector& connector) = default;
    Connector& operator=(Connector&& connector) noexcept = default;

    void connect(int index, const Connector::value_type& bus);

    [[nodiscard]] size_type count() const;

    Bus& getBus(size_type n);

    [[nodiscard]] const Bus& getBus(size_type n) const;

    void setBus(size_type n, Bus*);
};

#endif //MODERN_CPP_DESIGN_CONNECTOR_H
