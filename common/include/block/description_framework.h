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

// Place instance type
struct Instance : public Marker {
    using marker = int;
};

// Place TType begin to end times
template<size_t begin, size_t end, typename TType>
struct Range : public Marker {
    using TPin = TType;
    static_assert(begin <= end, "begin can't be greater then end");
};

template <auto, auto, typename, template <auto, auto, typename...> typename TType>
struct IsRange : public std::false_type {};

template <auto begin, auto end, typename ...TArgs, template <auto, auto, typename...> typename TType>
struct IsRange<begin, end, TType<begin, end, TArgs...>, TType> : public std::true_type {};

template<typename TDescription, typename = void>
struct HasInputs : std::false_type {
    using TPins = void;
};

template<typename TDescription>
struct HasInputs<TDescription, typename std::enable_if<TDescription::TInputs>> : std::true_type {
    using TPins = typename TDescription::TInputs;
};

template<typename TDescription, typename _TInstance>
struct InputAccessor {
    using TPins = typename HasInputs<TDescription>::TPins;
    using TInstance = _TInstance;

    constexpr static bool hasPins = HasInputs<TDescription>::value;
};

template <typename, template <typename...> typename>
struct IsTypeInstance : public std::false_type {};

template <typename ...TArgs, template <typename...> typename TType>
struct IsTypeInstance<TType<TArgs...>, TType> : public std::true_type {};

}
