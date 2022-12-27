#include <ostream>

#include "core/core_error.h"

std::ostream& block_engine::core::operator<<(std::ostream& output, const CoreErrorType&) {
    output << "None";
    return output;
}

std::ostream& block_engine::core::operator<<(std::ostream& output, const CoreError& error) {
    output << "CoreError{type=" << error.type << ", reason=" << error.reason << "}";
    return output;
}
