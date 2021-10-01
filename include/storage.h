//
// Created by Stepan on 13.09.2021.
//

#ifndef MODERN_CPP_DESIGN_STORAGE_H
#define MODERN_CPP_DESIGN_STORAGE_H

#include <limits>
#include <vector>
#include <stdexcept>
#include "type_info.h"

class DataStorage;
class StaticDataStorage;

class Position {
    friend class StaticDataStorage;

    size_t shift;

    Position(size_t shift) : shift(shift) {}
public:
    Position(const Position& position) = default;
};

class DataRef {
    friend class DataStorage;

    char* mData;

    explicit DataRef(char* data) : mData(data) { }

public:
    DataRef(const DataRef& dataRef) = default;

    const TypeInfo& type_info() {
        return *reinterpret_cast<TypeInfo*>(mData);
    }

    void* data() {
        return mData + sizeof(TypeInfo);
    }
};

class DataStorage {
    std::vector<char> buffer;

public:
    DataStorage() = default;
    DataStorage(const DataStorage& dataStorage) = delete;
    DataStorage(DataStorage&& dataStorage) = default;

    DataRef push_back(const TypeInfo& info) {
        auto* new_back = static_cast<char*>(buffer.data() + buffer.size());
        buffer.resize(buffer.size() + sizeof(TypeInfo) + info.size());

        new (new_back)TypeInfo(info);
        return DataRef(new_back);
    }
};

class StaticDataStorage {
public:
    StaticDataStorage() = default;
    StaticDataStorage(const StaticDataStorage& dataStorage) = delete;
    StaticDataStorage(StaticDataStorage&& dataStorage) = default;

    template<typename DataType>
    struct Record {
        const std::type_info& info;
        DataType data;

        explicit Record(const DataType& data) : info(typeid(DataType)), data(data) {}
    };

    template<typename DataType>
    std::pair<Position, DataType&> push_back(DataType&& data) {
        auto data_idx = buffer.size();
        buffer.resize(data_idx + sizeof(Record<DataType>));

        auto& dataRef = new (buffer.data() + data_idx) Record<DataType>(std::forward<>(data));

        return { data_idx, dataRef };
    }

    template<typename DataType, typename... Args>
    Position emplace_back(Args... args) {
        return push_back<DataType>(args...);
    }

    template<typename DataType>
    Position emplace_back() {
        return push_back(DataType());
    }

    template<typename DataType>
    DataType& at(Position pos) {
        if (pos.shift + sizeof(Record<DataType>) > buffer.size()) {
            throw std::out_of_range(__PRETTY_FUNCTION__);
        }

        auto& record = *reinterpret_cast<Record<DataType>*>(buffer.data() + pos.shift);
        if (record.info != typeid(DataType)) {
            throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        return record.data;
    }

    void* data(Position pos) {
        struct Void {};
        if (pos.shift + sizeof(Record<Void>) > buffer.size()) {
            throw std::out_of_range(__PRETTY_FUNCTION__);
        }

        auto& record = *reinterpret_cast<Record<Void>*>(buffer.data() + pos.shift);
        return &record.data;
    }
private:
    std::vector<char> buffer;
};

#endif //MODERN_CPP_DESIGN_STORAGE_H
