//
// Created by Stepan on 10.12.2022.
//

#pragma once

namespace block_engine::block {

template<typename TType>
struct BusType {
    static_assert(false && sizeof(TType), "Please register type by BUS_TYPE() macro");
};

#define BUS_TYPE(type) \
template<> \
struct BusType<type> { \
    const char* name = #type; \
};

BUS_TYPE(int)
BUS_TYPE(double)

#undef BUS_TYPE

}
