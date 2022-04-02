//
// Created by Stepan on 02.04.2022.
//

#ifndef BLOCK_ENGINE_CORE_ERROR_H
#define BLOCK_ENGINE_CORE_ERROR_H

#include <string>

namespace block_engine::core {

enum CoreErrorType {
    None,
};

struct CoreError {
    CoreErrorType type{None};
    std::string reason;
};

std::ostream &operator<<( std::ostream &output, const CoreErrorType &error_type) {
    output << "None";
    return output;
}

std::ostream &operator<<( std::ostream &output, const CoreError &error) {
    output << "CoreError{type=" << error.type << ", reason=" << error.reason << "}";
    return output;
}

}

#endif //BLOCK_ENGINE_CORE_ERROR_H
