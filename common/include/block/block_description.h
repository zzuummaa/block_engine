//
// Created by Stepan on 10.12.2022.
//

#pragma once

#include <tuple>

namespace block_engine::block {

struct ConsoleBlockDescription {
    using TInstances = std::tuple<int, double>;

    const char* name = "console";
};

struct ConstBlockDescription {
    using TInstances = std::tuple<int, double>;

    const char* name = "const";
};

struct LimitBlockDescription {
    using TInstances = std::tuple<int, double>;

    const char* name = "limit";
};

struct SumBlockDescription {
    using TInstances = std::tuple<int, double>;

    const char* name = "sum";
};

}