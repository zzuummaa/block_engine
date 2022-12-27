//
// Created by Stepan on 30.12.2021.
//

#ifndef BLOCK_ENGINE_GRAPH_H
#define BLOCK_ENGINE_GRAPH_H

#include <vector>
#include <algorithm>

#include "model/scheme.h"

namespace block_engine::graph {

inline void dfs(
    model::TBlockId blockId,
    const std::map<model::TBlockId, std::vector<model::TBlockId>>& adjList,
    std::set<model::TBlockId>& isVisit,
    std::vector<model::TBlockId>& sequence) {

    if (!isVisit.emplace(blockId).second) return;

    const auto it = adjList.find(blockId);
    if (it == adjList.end()) throw std::runtime_error(__PRETTY_FUNCTION__);

    for (const auto& nextBlockId : it->second) {
        dfs(nextBlockId, adjList, isVisit, sequence);
    }

    sequence.push_back(blockId);
}

inline std::vector<model::TBlockId> topologySort(const model::Scheme& scheme) {
    std::map<model::TBlockId, std::vector<model::TBlockId>> adjList;

    for (const auto& block : scheme.getBlocks()) {
        adjList.emplace(block.second.id, std::vector<model::TBlockId>());
    }

    for (const auto& link : scheme.links) {
        auto fromBlock = scheme.getBlockByPinId(link.output);
        auto toBlock = scheme.getBlockByPinId(link.input);

        if (!fromBlock || !toBlock) throw std::runtime_error(__PRETTY_FUNCTION__);

        auto it = adjList.find(fromBlock->id);
        if (it == adjList.end()) throw std::runtime_error(__PRETTY_FUNCTION__);

        it->second.push_back(toBlock->id);
    }

    for (auto& [_, adj] : adjList) {
        std::sort(adj.begin(), adj.end());
        adj.erase(std::unique(adj.begin(), adj.end()), adj.end());
    }

    std::vector<model::TBlockId> sequence;
    sequence.reserve(scheme.getBlocks().size());

    std::set<model::TBlockId> isVisit;

    for (const auto& [blockId, adj] : adjList) {
        dfs(blockId, adjList, isVisit, sequence);
    }

    std::reverse(sequence.begin(), sequence.end());
    return sequence;
}

}

#endif //BLOCK_ENGINE_GRAPH_H
