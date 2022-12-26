#ifndef BLOCK_ENGINE_MODEL_BLOCK_TYPE_INFO_H
#define BLOCK_ENGINE_MODEL_BLOCK_TYPE_INFO_H

#include <string>

namespace block_engine::model {

using TTypeId = int;

struct BlockTypeInfo {
    std::string name;
};

}

#endif //BLOCK_ENGINE_MODEL_BLOCK_TYPE_INFO_H
