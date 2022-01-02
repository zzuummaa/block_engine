//
// Created by Stepan on 30.09.2021.
//

#ifndef BLOCK_ENGINE_JSON_SCHEME_PARSER_H
#define BLOCK_ENGINE_JSON_SCHEME_PARSER_H

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <istream>

#include "model/scheme.h"

class JsonSchemeParser {
    nlohmann::json json_object;
public:
    explicit JsonSchemeParser(std::istream &is) : json_object(nlohmann::json::parse(is)) {}

    model::Scheme parse() {
        model::Scheme scheme;

        for (auto& json_type : json_object.at("types")) {
            auto id = json_type["id"].get<int>();
            if (!scheme.addType(id, json_type["name"].get<std::string>())) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
        }

        for (auto& json_type : json_object.at("block_types")) {
            auto id = json_type["id"].get<int>();
            if (!scheme.addBlockType(id, json_type["name"].get<std::string>())) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
        }

        for (auto& json_block : json_object.at("blocks")) {
            auto id = json_block["block_id"].get<int>();
            auto block = model::Block{
                .block_type_id = json_block["block_type_id"].get<int>(),
                .input_count = json_block["input_count"].get<int>(),
                .output_count = json_block["output_count"].get<int>()
            };
            if (!scheme.addBlock(id, block)) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        for (auto& json_link : json_object.at("links")) {
            auto link = model::Link{
                .type_id = json_link["type_id"].get<int>(),
                .block_out_id = json_link["block_out_id"].get<int>(),
                .bus_out_idx = json_link["bus_out_idx"].get<int>(),
                .block_in_id = json_link["block_in_id"].get<int>(),
                .bus_in_idx = json_link["bus_in_idx"].get<int>(),
            };
            if (!scheme.addLink(link)) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        return scheme;
    }
};

#endif //BLOCK_ENGINE_JSON_SCHEME_PARSER_H
