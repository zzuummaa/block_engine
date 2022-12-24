//
// Created by Stepan on 11.12.2022.
//

#pragma once

#include <qstring.h>

struct BusTypeInfo {
    QString name;
};

inline bool operator==(const BusTypeInfo& lhs, const BusTypeInfo& rhs) {
    return lhs.name == rhs.name;
}

inline bool operator!=(const BusTypeInfo& lhs, const BusTypeInfo& rhs) {
    return !(rhs == lhs);
}
