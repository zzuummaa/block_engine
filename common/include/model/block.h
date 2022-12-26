//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_BLOCK_H
#define BLOCK_ENGINE_BLOCK_H

#include <map>
#include <string>

#include "bus.h"

namespace block_engine::model {

using TBlockId = int;
using TBlockTypeId = int;

struct Block {
    TBlockId id;
    TBlockTypeId block_type_id;

    std::vector<Pin> inputs;
    std::vector<Pin> outputs;

    std::map<std::string, std::string> settings;
};

}
#endif //BLOCK_ENGINE_BLOCK_H
