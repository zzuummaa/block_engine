//
// Created by Stepan on 27.09.2021.
//

#ifndef BLOCK_ENGINE_TYPE_INFO_H
#define BLOCK_ENGINE_TYPE_INFO_H

#include <typeinfo.h>

class TypeInfo {
    const std::type_info& info;
    std::size_t size_;
public:

    TypeInfo(const std::type_info &info, size_t size) : info(info), size_(size) {}
    TypeInfo(const TypeInfo&) = default;

    [[nodiscard]] const char* name() const {
        return info.name();
    }

    [[nodiscard]] size_t size() const {
        return size_;
    }

    [[nodiscard]] size_t hash_code() const {
        return info.hash_code();
    }

    [[nodiscard]] const std::type_info& native_type_info() const {
        return info;
    }
};

#endif //BLOCK_ENGINE_TYPE_INFO_H
