//
// Created by Stepan on 31.12.2021.
//

#ifndef BLOCK_ENGINE_SCHEME_VALIDATOR_H
#define BLOCK_ENGINE_SCHEME_VALIDATOR_H

#include "model/scheme.h"

class SchemeValidator {
public:
    bool validate(const block_engine::model::Scheme& /*scheme*/) {
        return true;
    }
};

#endif //BLOCK_ENGINE_SCHEME_VALIDATOR_H
