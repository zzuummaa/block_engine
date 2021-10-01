//
// Created by Stepan on 30.09.2021.
//

#ifndef BLOCK_ENGINE_JSON_SCHEME_PARSER_H
#define BLOCK_ENGINE_JSON_SCHEME_PARSER_H

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <istream>

#include "link_engine.h"

template<typename BlockFactory, typename TypeFactory>
class JsonSchemeParser {
    nlohmann::json json_object;
    BlockFactory block_factory;
    TypeFactory type_factory;
public:
    explicit JsonSchemeParser(std::istream &is) : json_object(nlohmann::json::parse(is)) {}

    void parse(LinkEngine& linkEngine) {
        std::unordered_map<int, TypeInfo> typeMap;

        for (auto& json_type : json_object.at("types")) {
            auto id = json_type["id"].get<int>();
            if (typeMap.find(id) != typeMap.end()) throw std::runtime_error(__PRETTY_FUNCTION__);
            typeMap[id] = type_factory.createTypeInfoByName(json_type["name"].get<std::string>());
        }

        for (auto& json_block : json_object.at("blocks")) {
            linkEngine.addBlock(
                json_block["block_id"].get<int>(),
                block_factory.createBlockByName(
                    json_block["block_type"].get<std::string>()
                ),
                json_block["input_count"].get<int>(),
                json_block["output_count"].get<int>()
            );
        }

        for (auto& json_link : json_object.at("links")) {
            linkEngine.addLink(
                json_link["block_out_id"].get<int>(),
                json_link["block_out_idx"].get<int>(),
                json_link["block_in_id"].get<int>(),
                json_link["bus_in_idx"].get<int>(),
                typeMap.at(json_link["type_id"].get<int>())
            );
        }
    }
};

#endif //BLOCK_ENGINE_JSON_SCHEME_PARSER_H
