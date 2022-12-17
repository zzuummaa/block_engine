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

    BusGroupHolder() : isOptional(false) {}
    BusGroupHolder(BusGroupHolder&&) = default;
    BusGroupHolder(const BusGroupHolder&) = delete;

    BusGroupHolder& operator=(BusGroupHolder& other) = delete;
    BusGroupHolder& operator=(BusGroupHolder&& other) = default;

    template<typename TType>
    explicit BusGroupHolder(bool isOptional) : value(TType()), isOptional(isOptional) {}

    template<typename TType>
    BusGroupHolder(TType&& value, bool isOptional) : value(std::forward<TType>(value)), isOptional(isOptional) {}

    [[nodiscard]] constexpr bool isEmpty() const noexcept {
        return std::holds_alternative<std::monostate>(value);
    }

    [[nodiscard]] constexpr bool isBus() const noexcept {
        return std::holds_alternative<BusGroupHolder::TBus>(value);
    }

    [[nodiscard]] constexpr bool isBusCollection() const noexcept {
        return std::holds_alternative<BusGroupHolder::TBusCollection>(value);
    }

    [[nodiscard]] constexpr bool isHolderCollection() const noexcept {
        return std::holds_alternative<BusGroupHolder::THolderCollection>(value);
    }

    TBus bus();
    TBusCollection& busCollection();
    THolderCollection& holderCollection();

    template<typename TConsumer>
    void forEach(const TConsumer& consumer) {
        forEach(*this, consumer);
    }

    template<typename TConsumer>
    static void forEach(BusGroupHolder& holder, const TConsumer& consumer) {
        if (holder.isBus()) {
            consumer(holder.bus());
        } else if (holder.isBusCollection()) {
            auto& pins = holder.busCollection();
            std::for_each(pins.begin(), pins.end(), consumer);
        } else if (holder.isHolderCollection()) {
            auto& holders = holder.holderCollection();
            for (auto& subHolder : holders) {
                forEach(*subHolder, consumer);
            }
        }
    }

private:
    std::variant<std::monostate, TBus, TBusCollection, THolderCollection> value;
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