//
// Created by Stepan on 26.09.2021.
//

#ifndef MODERN_CPP_DESIGN_BUS_H
#define MODERN_CPP_DESIGN_BUS_H

#include <stdexcept>
#include <vector>
#include <functional>
#include <memory>

#include "type_info.h"

namespace block_engine::core {

struct Bus {
    TypeInfo type_info;
    std::shared_ptr<void> data_;

    Bus(const Bus& other) = default;
    Bus(Bus&&) = default;

    template<typename TDataType>
    explicit Bus(const TDataType& value) : type_info(make_type_info<TDataType>()) {
        data_ = std::make_shared<TDataType>(value);
    }

    template<typename TDataType>
    TDataType& data_unchecked() {
        return *std::static_pointer_cast<TDataType>(data_);
    }

    template<typename TDataType>
    TDataType& data() {
        if (type_info != make_type_info<TDataType>()) throw std::runtime_error(__PRETTY_FUNCTION__);
        return data_unchecked<TDataType>();
    }
};

using BusPtr = std::shared_ptr<Bus>;

template<typename... Args>
BusPtr makeBusPtr(Args&&... args) {
    return std::make_shared<Bus>(std::forward<Args>(args)...);
}

}

#endif //MODERN_CPP_DESIGN_BUS_H
