#include "block_type_info.h"

bool operator<(const BlockTypeInfo& lhs, const BlockTypeInfo& rhs) {
    return lhs.name < rhs.name;
}
