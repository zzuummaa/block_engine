//
// Created by Stepan on 30.12.2021.
//

#ifndef BLOCK_ENGINE_GRAPH_H
#define BLOCK_ENGINE_GRAPH_H

#include <vector>
#include <algorithm>

#include "model/scheme.h"

namespace graph {

    struct DefaultSchemeExtractorPolicy {
        typedef decltype(model::Scheme().blocks)::key_type TBlockKey;

        static size_t blocksCount(const model::Scheme& scheme) {
            return scheme.blocks.size();
        }

        static auto blocksBegin(const model::Scheme& scheme) {
            return scheme.blocks.begin();
        }

        static auto blocksEnd(const model::Scheme& scheme) {
            return scheme.blocks.end();
        }

        static auto linksBegin(const model::Scheme& scheme, const TBlockKey& id) {
            return scheme.links.lower_bound({ .block_out_id = id });
        }

        static auto linksEnd(const model::Scheme& scheme, const TBlockKey& id) {
            return scheme.links.lower_bound({ .block_out_id = id + 1 });
        }

        static TBlockKey blockKey(const model::Link& link) {
            return link.block_in_id;
        }

        static TBlockKey blockKey(const std::pair<TBlockKey, model::Block>& pair) {
            return pair.first;
        }
    };

    template<typename TScheme, typename TExtractorPolicy>
    void dfs(typename TExtractorPolicy::TBlockKey blockKey,
             const TScheme& scheme,
             std::set<typename TExtractorPolicy::TBlockKey> &is_visit,
             std::vector<typename TExtractorPolicy::TBlockKey> &sequence) {

        if (!is_visit.emplace(blockKey).second) return;

        for (auto it = TExtractorPolicy::linksBegin(scheme, blockKey); it != TExtractorPolicy::linksEnd(scheme, blockKey); it++) {
            dfs<TScheme, TExtractorPolicy>(TExtractorPolicy::blockKey(*it), scheme, is_visit, sequence);
        }

        sequence.push_back(blockKey);
    }

    template<typename TScheme, typename TExtractorPolicy = DefaultSchemeExtractorPolicy>
    std::vector<typename TExtractorPolicy::TBlockKey> topologySort(const TScheme& scheme) {
        auto blocksCount = TExtractorPolicy::blocksCount(scheme);

        std::vector<typename TExtractorPolicy::TBlockKey> sequence;
        sequence.reserve(blocksCount);

        std::set<typename TExtractorPolicy::TBlockKey> is_visit;

        for (auto it = TExtractorPolicy::blocksBegin(scheme); it != TExtractorPolicy::blocksEnd(scheme); it++) {
            dfs<TScheme, TExtractorPolicy>(TExtractorPolicy::blockKey(*it), scheme, is_visit, sequence);
        }

        std::reverse(sequence.begin(), sequence.end());
        return sequence;
    }

}

#endif //BLOCK_ENGINE_GRAPH_H
