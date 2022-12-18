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

    block_engine::model::Scheme parse() {
        model::Scheme scheme;

        for (auto &json_type: json_object.at("types")) {
            auto id = json_type["id"].get<int>();
            if (!scheme.addType(id, json_type["name"].get<std::string>())) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
        }

        for (auto &json_type: json_object.at("block_types")) {
            auto id = json_type["id"].get<int>();
            if (!scheme.addBlockType(id, json_type["name"].get<std::string>())) {
                throw std::runtime_error(__PRETTY_FUNCTION__);
            }
        }

        for (auto &json_block: json_object.at("blocks")) {
            auto block = model::Block{
                .id = json_block["block_id"].get<int>(),
                .block_type_id = json_block["block_type_id"].get<int>(),
                .input_count = json_block["input_count"].get<int>(),
                .output_count = json_block["output_count"].get<int>()
            };
            if (!scheme.addBlock(block.id, block)) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        for (auto& json_bus: json_object.at("busses")) {
            if (!scheme.addBus(getBus(json_bus))) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        return scheme;
    }

    static model::Bus getBus(const nlohmann::json& json_bus) {
        auto bus = model::Bus{
            .type_id = json_bus["type_id"].get<int>(),
            .src = {
                .block_id = json_bus["src_block_id"].get<int>(),
                .pin_idx = json_bus["src_pin_idx"].get<int>()
            }
        };
        for (auto& dest_pin: json_bus.at("dest_pins")) {
            auto pin = model::Pin{
                .block_id = dest_pin["block_id"].get<int>(),
                .pin_idx = dest_pin["pin_idx"].get<int>()
            };
            bus.dests.push_back(pin);
        }
        return bus;
    }

};

}

#endif //BLOCK_ENGINE_JSON_SCHEME_PARSER_H
