//
// Created by Stepan on 11.12.2022.
//

#pragma once

#include <tuple>

namespace block_engine::block {

template <typename TWrapper, typename T>
struct Instances {
    static_assert(false && sizeof(T), "template parameter should be std::tuple");
};

template <typename TWrapper, typename ...TArgs>
struct Instances<TWrapper, std::tuple<TArgs...>> : public std::tuple<TArgs...> {

};

struct Marker {};

struct Empty : public Marker {};

// Place instance type
struct Instance : public Marker {};

// Place TType begin to end times
template<size_t begin_, size_t end_, typename TType>
struct Range : public Marker {
    static constexpr int begin = begin_;
    static constexpr int end = end_;
    using TPin = TType;
    static_assert(begin <= end, "begin can't be greater then end");
};

template <typename, template <auto, auto, typename...> typename TType>
struct IsRange : public std::false_type {};

template <auto begin, auto end, typename ...TArgs, template <auto, auto, typename...> typename TType>
struct IsRange<TType<begin, end, TArgs...>, TType> : public std::true_type {};

template<typename TDescription, typename = void>
struct HasInputs : std::false_type {
    using TPins = Empty;
};

template<typename TDescription>
struct HasInputs<TDescription, std::void_t<typename TDescription::TInputs>> : std::true_type {
    using TPins = typename TDescription::TInputs;
};

template<typename TDescription, typename TInstance_>
struct InputAccessor {
    using TPins = typename HasInputs<TDescription>::TPins;
    using TInstance = TInstance_;

    static constexpr bool hasPins = HasInputs<TDescription>::value;
    static constexpr bool isInput = true;
};

template <typename, template <typename...> typename>
struct IsTypeInstance : public std::false_type {};

template <typename ...TArgs, template <typename...> typename TType>
struct IsTypeInstance<TType<TArgs...>, TType> : public std::true_type {};

}
