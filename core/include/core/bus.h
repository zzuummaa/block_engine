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

struct Bus {
    TypeInfo type_info;
    std::vector<char> data_;
    std::function<void()> destroyer;

    Bus() = default;
    Bus(const Bus& other) = default;
    Bus(Bus&&) = default;

    template<typename TDataType>
    explicit Bus(const TDataType& value) : type_info(make_type_info<TDataType>()), data_(sizeof(value)) {
        *reinterpret_cast<TDataType*>(data_.data()) = value;
        destroyer = [this](){
            auto& data_value = data_unchecked<TDataType>();
            data_value.~TDataType();
        };
    }

    ~Bus() {
        if (destroyer) destroyer();
    }

    template<typename TDataType>
    TDataType& data_unchecked() {
        auto* start_data = data_.data();
        auto* typed_data = reinterpret_cast<TDataType*>(start_data);
        return *typed_data;
    }

    template<typename TDataType>
    TDataType& data() {
        if (type_info != make_type_info<TDataType>()) throw std::runtime_error(__PRETTY_FUNCTION__);
        return data_unchecked<TDataType>();
    }
};

namespace block_engine::core {
    typedef std::shared_ptr<Bus> BusPtr;

    template<typename... Args>
    BusPtr makeBusPtr(Args&&... args) {
        return std::make_shared<Bus>(std::forward<Args>(args)...);
    }
}

#endif //MODERN_CPP_DESIGN_BUS_H
