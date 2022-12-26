//
// Created by Stepan on 30.09.2021.
//

#ifndef BLOCK_ENGINE_JSON_SCHEME_PARSER_H
#define BLOCK_ENGINE_JSON_SCHEME_PARSER_H

#include <nlohmann/json.hpp>
#include <unordered_map>
#include <istream>

#include "model/scheme.h"

namespace block_engine::view::json {

class JsonSchemeParser {
    nlohmann::json json_object;
public:
    explicit JsonSchemeParser(std::istream &is) : json_object(nlohmann::json::parse(is)) {}

    auto parsePin(nlohmann::json& json_pin, const model::Block& block, bool isInput) {
        return model::Pin {
            .id = model::TPinId(block.id, isInput, json_pin["idx"].get<int>()),
            .typeId = json_pin["type_id"].get<model::TTypeId>()
        };
    }

    block_engine::model::Scheme parse() {
        model::Scheme scheme;

        for (auto& json_type: json_object.at("types")) {
            auto id = json_type["id"].get<int>();
            if (!scheme.addType(id, json_type["name"].get<std::string>())) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
        }

        for (auto& json_type: json_object.at("block_types")) {
            auto id = json_type["id"].get<int>();
            if (!scheme.addBlockType(id, json_type["name"].get<std::string>())) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
        }

        for (auto& json_block: json_object.at("blocks")) {
            auto block = model::Block{
                .id = json_block["block_id"].get<int>(),
                .block_type_id = json_block["block_type_id"].get<int>()
            };
            for (auto& json_input : json_block.at("inputs")) {
                block.inputs.push_back(parsePin(json_input, block, true));
            }
            for (auto& json_output : json_block.at("outputs")) {
                block.outputs.push_back(parsePin(json_output, block, false));
            }
            if (!scheme.addBlock(block.id, block)) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        for (auto& json_link : json_object.at("links")) {
            auto output = model::TPinId(
                json_link["block_out_id"].get<model::TBlockId>(),
                false,
                json_link["bus_out_idx"].get<model::TBlockId>()
            );

            auto input = model::TPinId(
                json_link["block_in_id"].get<model::TBlockId>(),
                true,
                json_link["bus_in_idx"].get<model::TBlockId>()
            );

            auto link = model::Link{
                .output = output,
                .input = input
            };
            if (!scheme.addLink(link)) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        return scheme;
    }

};

}

#endif //BLOCK_ENGINE_JSON_SCHEME_PARSER_H
