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

    explicit DefaultConnectionPolicy(BusFactory bus_factory) : bus_factory(std::move(bus_factory)) {}

    auto connectors(const model::Scheme& scheme) {
        std::map<model::TBlockId, ConnectorPair> conns;
        std::map<model::TPinId, BusPtr> busByOutput;

        for (const auto& [id, block]: scheme.getBlocks()) {
            const auto& [_, is_ok] = conns.emplace(
                std::remove_const<decltype(id)>::type(id),
                std::make_pair(Connector(block.inputs.size()), Connector(block.outputs.size()))
            );
            if (!is_ok) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        for (const auto& link : scheme.links) {
            const auto busIt = busByOutput.find(link.output);

            BusPtr bus;
            if (busIt == busByOutput.end()) {
                const auto type = scheme.getTypeByPinId(link.output);
                if (!type) throw std::invalid_argument(__PRETTY_FUNCTION__);
                bus = busByOutput.emplace(link.output, bus_factory.createBusPtrByName(*type)).first->second;
            } else {
                bus = busIt->second;
            }

            auto outputConnectorPair = conns.find(link.output.getBlockId());
            if (outputConnectorPair == conns.end()) throw std::runtime_error(__PRETTY_FUNCTION__);
            Connector::output(outputConnectorPair->second).setBusIfNotSet(link.output.getPinIdx(), bus);

            auto inputConnectorPair = conns.find(link.input.getBlockId());
            if (inputConnectorPair == conns.end()) throw std::runtime_error(__PRETTY_FUNCTION__);
            Connector::input(inputConnectorPair->second).setBusIfNotSet(link.input.getPinIdx(), bus);
        }

        for (const auto& [_, connectorPair] : conns) {
            if (!Connector::input(connectorPair).isLinked()
            ||  !Connector::output(connectorPair).isLinked()) throw std::runtime_error(__PRETTY_FUNCTION__);
        }

        return conns;
    }

private:
    BusFactory bus_factory;
};

struct DefaultBlockPolicy {
   explicit DefaultBlockPolicy(BlockFactory block_factory) : block_factory(std::move(block_factory)) {}

   auto blocks(const model::Scheme& scheme) {
        std::map<int, BlockLogicBasePtr> blocks;
        std::transform(
            scheme.getBlocks().begin(),
            scheme.getBlocks().end(),
            std::inserter(blocks, blocks.end()),
            [&](auto& it){
                const auto blockName = scheme.getBlockType(it.second.block_type_id);
                if (!blockName) throw std::runtime_error(__PRETTY_FUNCTION__);
                return std::make_pair(
                    it.first,
                    block_factory.createBlockByName(*blockName)
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
