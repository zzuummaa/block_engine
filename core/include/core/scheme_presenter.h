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

struct OutputPinLinkLess {
    bool operator()(const model::Link& lhs, const model::Link& rhs) const {
        if (lhs.block_out_id < rhs.block_out_id)
            return true;
        if (rhs.block_out_id < lhs.block_out_id)
            return false;
        if (lhs.bus_out_idx < rhs.bus_out_idx)
            return true;
        return lhs.bus_out_idx > rhs.bus_out_idx;
    }
};

struct DefaultConnectionPolicy {
    typedef Connector TConnector;

    explicit DefaultConnectionPolicy(BusFactory bus_factory) : bus_factory(std::move(bus_factory)) {}

    auto makeLinkBusMapping(const model::Scheme& scheme) {
        std::map<model::Link, BusPtr, OutputPinLinkLess> link_bus_mapping;

        for (const auto& link: scheme.links) {
            auto it = link_bus_mapping.find(link);
            if (it != link_bus_mapping.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);

            const auto& type = scheme.types.find(link.type_id);
            if (type == scheme.types.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
            link_bus_mapping.emplace(link, bus_factory.createBusPtrByName(type->second));
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

        for (const auto& link: scheme.links) {
            auto in_block_it = conns.find(link.block_in_id);
            if (in_block_it == conns.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
            auto& in_connector = TConnector::input(in_block_it->second);

            in_connector.setBus(link.bus_in_idx, link_bus_mapping[link]);

            auto out_block_it = conns.find(link.block_out_id);
            if (out_block_it == conns.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
            auto& out_connector = TConnector::output(out_block_it->second);

            out_connector.setBus(link.bus_out_idx, link_bus_mapping[link]);
        }

        return conns;
    }

private:
    BusFactory bus_factory;
};

struct DefaultBlockPolicy {
   explicit DefaultBlockPolicy(BlockFactory block_factory) : block_factory(std::move(block_factory)) {}

   auto blocks(const model::Scheme& scheme) {
        std::map<int, IBlockLogic*> blocks;
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
