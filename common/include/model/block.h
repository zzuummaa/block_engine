//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_BLOCK_H
#define BLOCK_ENGINE_BLOCK_H

#include <map>
#include <string>

namespace block_engine::model {

struct Block {
    int id;
    int block_type_id;
    int input_count;
    int output_count;
    std::map<std::string, std::string> settings;
};

}
#endif //BLOCK_ENGINE_BLOCK_H
