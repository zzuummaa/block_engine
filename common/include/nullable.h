//
// Created by Stepan on 11.12.2022.
//

#pragma once

#include <optional>
#include <utility>

namespace block_engine {

template <typename T>
struct Nullable : public std::optional<T> {
    constexpr Nullable() = default;
    constexpr Nullable(std::nullopt_t value) noexcept : std::optional<T>(value) {}

    template<typename R>
    constexpr Nullable(R&& value) : std::optional<T>(std::forward<R>(value)) {}

    Nullable<T> operator+(const Nullable<T> &b) const {
        if (*this && b) {
            return **this + *b;
        } else {
            return std::nullopt;
        }
    }
};

}

