//
// Created by Stepan on 04.01.2022.
//

#ifndef BLOCK_ENGINE_SCHEME_PRESENTER_H
#define BLOCK_ENGINE_SCHEME_PRESENTER_H

#include "model/scheme.h"
#include "bus_factory.h"
#include "connector.h"
#include "block_factory.h"

#include <memory>

namespace block_engine::core {

struct DefaultConnectionPolicy {
    typedef Connector TConnector;

    explicit DefaultConnectionPolicy(BusFactory bus_factory) : bus_factory(std::move(bus_factory)) {}

    auto makeLinkBusMapping(const model::Scheme& scheme) {
        std::map<model::Bus, BusPtr, model::BusLessByBlockId> link_bus_mapping;

        for (const auto& bus: scheme.busses) {
            auto it = link_bus_mapping.find(bus);
            if (it != link_bus_mapping.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);

            const auto& type = scheme.types.find(bus.type_id);
            if (type == scheme.types.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
            link_bus_mapping.emplace(bus, bus_factory.createBusPtrByName(type->second));
        }

        return link_bus_mapping;
    }

    auto connectors(const model::Scheme& scheme) {
        std::map<int, std::pair<TConnector, TConnector>> conns;

        for (const auto& [id, block]: scheme.blocks) {
            const auto& [_, is_ok] = conns.emplace(
                std::remove_const<typeof(id)>::type(id),
                std::make_pair(TConnector(block.input_count), TConnector(block.output_count))
            );
            if (!is_ok) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        auto link_bus_mapping = makeLinkBusMapping(scheme);

        for (const auto& bus: scheme.busses) {
            auto src_block_it = conns.find(bus.src.block_id);
            if (src_block_it == conns.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
            auto& out_connector = TConnector::output(src_block_it->second);

            out_connector.setBusIfEmptyOrError(bus.src.pin_idx, link_bus_mapping[bus]);

            for (auto& dest_pin : bus.dests) {
                auto dst_block_it = conns.find(dest_pin.block_id);
                if (dst_block_it == conns.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
                auto& in_connector = TConnector::input(dst_block_it->second);

                in_connector.setBusIfEmptyOrError(dest_pin.pin_idx, link_bus_mapping[bus]);
            }
        }

        return conns;
    }

private:
    BusFactory bus_factory;
};

struct DefaultBlockPolicy {
   explicit DefaultBlockPolicy(BlockFactory block_factory) : block_factory(std::move(block_factory)) {}

   auto blocks(const model::Scheme& scheme) {
        std::map<int, BlockLogicBase*> blocks;
        std::transform(
            scheme.blocks.begin(),
            scheme.blocks.end(),
            std::inserter(blocks, blocks.end()),
            [&](auto& it){
                return std::make_pair(
                    it.first,
                    block_factory.createBlockByName(*scheme.getBlockType(it.second.block_type_id))
                );
            }
        );

       return blocks;
   }

private:
    BlockFactory block_factory;
};

}

#endif //BLOCK_ENGINE_SCHEME_PRESENTER_H
