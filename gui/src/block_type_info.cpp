#include "gui/block_type_info.h"

bool block_engine::gui::operator<(const BlockTypeInfo& lhs, const BlockTypeInfo& rhs) {
    return lhs.name < rhs.name;
}
