//
// Created by Stepan on 11.12.2022.
//

#include <core/block/block.h>

namespace block_engine::core {

void BlockLogicBase::setBlock(const model::Block& block_) {
    this->block = block_;
    updateSettings();
}

}
