//
// Created by Stepan on 04.01.2022.
//

#ifndef BLOCK_ENGINE_BLOCK_CONNECTION_POLICY_H
#define BLOCK_ENGINE_BLOCK_CONNECTION_POLICY_H

#include "model/scheme.h"
#include "bus.h"
#include "connector.h"

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

template<typename TBusFactory>
struct DefaultConnectionPolicy {
    static auto connectors(const model::Scheme& scheme) {
        std::map<int, std::pair<Connector, Connector>> conns;

        for (const auto& [id, block]: scheme.blocks) {
            const auto& [_, is_ok] = conns.emplace(
                std::remove_const<typeof(id)>::type(id),
                std::make_pair(Connector(block.input_count), Connector(block.output_count))
            );
            if (!is_ok) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        std::map<model::Link, Bus*, OutputPinLinkLess> unique_busses;
        for (const auto& link: scheme.links) {
            auto it = unique_busses.find(link);
            if (it == unique_busses.end()) {
                const auto& type = scheme.types.find(link.type_id);
                if (type == scheme.types.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
                unique_busses.emplace(link, new Bus(TBusFactory::createBus(type->second)));
            }
        }

        for (const auto& link: scheme.links) {
            auto in_block_it = conns.find(link.block_in_id);
            if (in_block_it == conns.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
            auto& in_connector = Connector::input(in_block_it->second);

            in_connector.setBus(link.bus_in_idx, unique_busses[link]);

            auto out_block_it = conns.find(link.block_out_id);
            if (out_block_it == conns.end()) throw std::invalid_argument(__PRETTY_FUNCTION__);
            auto& out_connector = Connector::output(out_block_it->second);

            out_connector.setBus(link.bus_out_idx, unique_busses[link]);
        }

        return conns;
    }
};

template<typename TBlockFactory>
struct DefaultBlockPolicy {
   static auto blocks(const model::Scheme& scheme) {
        std::map<int, IBlock*> blocks;
        std::transform(
            scheme.blocks.begin(),
            scheme.blocks.end(),
            std::inserter(blocks, blocks.end()),
            [&scheme](auto& it){
                return std::make_pair(
                    it.first,
                    TBlockFactory::createBlockByName(*scheme.getBlockType(it.second.block_type_id))
                );
            }
        );

       return blocks;
   }
};

#endif //BLOCK_ENGINE_BLOCK_CONNECTION_POLICY_H
