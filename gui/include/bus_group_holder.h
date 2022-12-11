//
// Created by Stepan on 11.12.2022.
//

#pragma once

#include <qbus.h>
#include <variant>

class BusGroupHolder {
public:
    using TBus = QBus*;
    using TBusCollection = std::vector<TBus>;
    using THolderCollection = std::vector<std::unique_ptr<BusGroupHolder>>;

    template<typename TType>
    explicit BusGroupHolder(bool isOptional) : value(TType()), isOptional(isOptional) {}

    template<typename TType>
    BusGroupHolder(const TType& value, bool isOptional) : value(value), isOptional(isOptional) {}

    [[nodiscard]] bool isBus() const;
    [[nodiscard]] bool isBusCollection() const;
    [[nodiscard]] bool isHolderCollection() const;

    TBus bus();
    TBusCollection& busCollection();
    THolderCollection& holderCollection();

private:
    std::variant<TBus, TBusCollection, THolderCollection> value;
    bool isOptional;

    template<typename TType>
    TType& get() {
        try {
            return std::get<TType>(value);
        } catch (const std::bad_variant_access& e) {
            std::throw_with_nested(std::runtime_error(std::string(e.what()) + "; " + __PRETTY_FUNCTION__));
        }
    }
};