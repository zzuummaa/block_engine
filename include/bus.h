//
// Created by Stepan on 26.09.2021.
//

#ifndef MODERN_CPP_DESIGN_BUS_H
#define MODERN_CPP_DESIGN_BUS_H

#include <cstring>
#include "type_info.h"

struct Bus {
    void* data_;

    Bus() : data_(nullptr) {}
    explicit Bus(void* data) : data_(data) {}

    Bus(const Bus& other) = default;

    template<typename DataType>
    DataType& data_unchecked() {
        auto* start_data = static_cast<char*>(data_) + sizeof(TypeInfo);
        auto* typed_data = reinterpret_cast<DataType*>(start_data);
        return *typed_data;
    }
};

#endif //MODERN_CPP_DESIGN_BUS_H
