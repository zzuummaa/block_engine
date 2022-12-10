//
// Created by Stepan on 10.12.2022.
//

#pragma once

#include <tuple>
#include <nullable.h>

namespace block_engine::block {

using TCommonTypes = std::tuple<int, double, Nullable<int>, Nullable<double>>;

struct ConsoleBlockDescription {
    using TInstances = TCommonTypes;

    const char* name = "console";
};

struct ConstBlockDescription {
    using TInstances = TCommonTypes;

    const char* name = "const";
};

struct LimitBlockDescription {
    using TInstances = TCommonTypes;

    const char* name = "limit";
};

struct SumBlockDescription {
    using TInstances = TCommonTypes;

    const char* name = "sum";
};

}