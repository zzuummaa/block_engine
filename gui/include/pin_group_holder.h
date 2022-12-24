//
// Created by Stepan on 11.12.2022.
//

#pragma once

#include <qpin.h>
#include <variant>

class PinGroupHolder {
public:
    using TPin = QPin*;
    using TPinCollection = std::vector<TPin>;
    using THolderCollection = std::vector<std::unique_ptr<PinGroupHolder>>;

    PinGroupHolder() : isOptional(false) {}
    PinGroupHolder(PinGroupHolder&&) = default;
    PinGroupHolder(const PinGroupHolder&) = delete;

    PinGroupHolder& operator=(PinGroupHolder& other) = delete;
    PinGroupHolder& operator=(PinGroupHolder&& other) = default;

    template<typename TType>
    explicit PinGroupHolder(bool isOptional) : value(TType()), isOptional(isOptional) {}

    template<typename TType>
    PinGroupHolder(TType&& value, bool isOptional) : value(std::forward<TType>(value)), isOptional(isOptional) {}

    [[nodiscard]] constexpr bool isEmpty() const noexcept {
        return std::holds_alternative<std::monostate>(value);
    }

    [[nodiscard]] constexpr bool isBus() const noexcept {
        return std::holds_alternative<PinGroupHolder::TPin>(value);
    }

    [[nodiscard]] constexpr bool isBusCollection() const noexcept {
        return std::holds_alternative<PinGroupHolder::TPinCollection>(value);
    }

    [[nodiscard]] constexpr bool isHolderCollection() const noexcept {
        return std::holds_alternative<PinGroupHolder::THolderCollection>(value);
    }

    TPin bus();
    TPinCollection& busCollection();
    THolderCollection& holderCollection();

    template<typename TConsumer>
    void forEach(const TConsumer& consumer) {
        forEach(*this, consumer);
    }

    template<typename TConsumer>
    static void forEach(PinGroupHolder& holder, const TConsumer& consumer) {
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
    std::variant<std::monostate, TPin, TPinCollection, THolderCollection> value;
    [[maybe_unused]] bool isOptional;

    template<typename TType>
    TType& get() {
        try {
            return std::get<TType>(value);
        } catch (const std::bad_variant_access& e) {
            std::throw_with_nested(std::runtime_error(std::string(e.what()) + "; " + __PRETTY_FUNCTION__));
        }
    }
};