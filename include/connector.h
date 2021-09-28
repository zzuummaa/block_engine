//
// Created by Stepan on 25.09.2021.
//

#ifndef MODERN_CPP_DESIGN_CONNECTOR_H
#define MODERN_CPP_DESIGN_CONNECTOR_H

#include <vector>
#include "bus.h"

struct Connector : private std::vector<Bus> {
public:
    Connector() = default;
    explicit Connector(unsigned long long int count) : vector(count) {}

    void connect(int index, const Bus& bus);

    const std::vector<Bus>& getBusses();
};

#endif //MODERN_CPP_DESIGN_CONNECTOR_H
