#ifndef BLOCK_ENGINE_BLOCK_TYPE_INFO_H
#define BLOCK_ENGINE_BLOCK_TYPE_INFO_H

#include <QString>

namespace block_engine::gui {

struct BlockTypeInfo {
    QString name;
};

bool operator<(const BlockTypeInfo& lhs, const BlockTypeInfo& rhs);

}

#endif //BLOCK_ENGINE_BLOCK_TYPE_INFO_H
