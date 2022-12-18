//
// Created by Stepan on 10.12.2022.
//

#pragma once

#include <iostream>
#include "nullable.h"

namespace block_engine {

template<typename TType>
struct BusType {
    static_assert(false && sizeof(TType), "Please register type by BUS_TYPE() macro");
};

#define BUS_TYPE(type) \
template<> \
struct BusType<type> { \
    const char* name = #type; \
};

/*
 * Concatenate preprocessor tokens A and B without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define PPCAT_NX(A, B) A ## B

/*
 * Concatenate preprocessor tokens A and B after macro-expanding them.
 */
#define PPCAT(A, B) PPCAT_NX(A, B)

/*
 * Turn A into a string literal without expanding macro definitions
 * (however, if invoked from a macro, macro arguments are expanded).
 */
#define STRINGIZE_NX(A) #A

/*
 * Turn A into a string literal after macro-expanding it.
 */
#define STRINGIZE(A) STRINGIZE_NX(A)

#define BUS_NULLABLE_TYPE(type) \
template<> \
struct BusType<Nullable<type>> \
{ \
    const char* name = STRINGIZE(PPCAT(nullable_, type)); \
};

#define BUS_NULLABLE_OSTREAM_OPERATOR(type) \
inline std::ostream& operator<<(std::ostream &output, const block_engine::Nullable<type>& value) { \
    if (value.has_value()) { \
        output << const_cast<type&>(*value); \
    } \
    else {                              \
        output << std::string_view("null"); \
    }\
    return output; \
}

#define BUS_REGISTER(type) \
    BUS_TYPE(type) \
    BUS_NULLABLE_TYPE(type) \
    BUS_NULLABLE_OSTREAM_OPERATOR(type)

BUS_REGISTER(int)
BUS_REGISTER(double)

#undef BUS_TYPE
#undef BUS_OPTIONAL_TYPE
#undef BUS_OSTREAM_OPERATOR

}
