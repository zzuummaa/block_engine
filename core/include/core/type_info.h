//
// Created by Stepan on 27.09.2021.
//

#ifndef BLOCK_ENGINE_TYPE_INFO_H
#define BLOCK_ENGINE_TYPE_INFO_H

#include <cassert>
#include <typeinfo.h>
#include <string>

class TypeInfo {
    const char* name_{};
    std::size_t hash_code_{};
    std::size_t type_size{};
public:

    TypeInfo() = default;
    TypeInfo(const TypeInfo& other) = default;

//    template<typename TData>
//    TypeInfo() : name_(typeid(TData).name()), hash_code_(typeid(TData).hash_code()), type_size(sizeof(TData)) {}

    TypeInfo& operator=(const TypeInfo& other) = default;

    explicit operator bool() const {
        return name_ != nullptr;
    }

    [[nodiscard]] const char* name() const {
        assert(*this);
        return name_;
    }

    [[nodiscard]] size_t size() const {
        assert(*this);
        return type_size;
    }

    [[nodiscard]] size_t hash_code() const {
        assert(*this);
        return hash_code_;
    }

    bool operator==(const TypeInfo &rhs) const {
        return hash_code_ == rhs.hash_code_;
    }

    bool operator!=(const TypeInfo &rhs) const {
        return !(rhs == *this);
    }

    template<typename TData>
    friend TypeInfo make_type_info();
};

template<typename TData>
TypeInfo make_type_info() {
    TypeInfo info;
    info.name_ = typeid(TData).name();
    info.hash_code_ = typeid(TData).hash_code();
    info.type_size = sizeof(TData);
    return info;
}

#endif //BLOCK_ENGINE_TYPE_INFO_H
