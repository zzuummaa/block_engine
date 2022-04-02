//
// Created by Stepan on 30.12.2021.
//

#ifndef BLOCK_ENGINE_GRAPH_H
#define BLOCK_ENGINE_GRAPH_H

#include <vector>
#include <algorithm>

#include "model/scheme.h"

namespace block_engine::graph {

struct PinIterator {
    typedef decltype(model::Scheme::busses.begin()) TBusIterator;
    typedef decltype(model::Bus::dests.cbegin()) TDestIterator;

    explicit PinIterator(TBusIterator bus_iterator_begin, TBusIterator bus_iterator_end, TDestIterator dest_iterator)
        : bus_iterator_begin(bus_iterator_begin), bus_iterator_end(bus_iterator_end), dest_iterator(dest_iterator) {}

    PinIterator operator++() {
        if (bus_iterator_begin == bus_iterator_end) throw std::runtime_error(__PRETTY_FUNCTION__);

        if (++dest_iterator == bus_iterator_begin->dests.end()) {
            return PinIterator(++bus_iterator_begin, bus_iterator_end, bus_iterator_begin->dests.begin());
        } else {
            return PinIterator(bus_iterator_begin, bus_iterator_end, dest_iterator);
        }
    }
    model::Pin operator*() const {
        if (bus_iterator_begin == bus_iterator_end) throw std::runtime_error(__PRETTY_FUNCTION__);
        return *dest_iterator;
    }

    const model::Pin* operator->() const {
        if (bus_iterator_begin == bus_iterator_end) throw std::runtime_error(__PRETTY_FUNCTION__);
        return dest_iterator.base();
    }

    friend bool operator==(const PinIterator& lhs, const PinIterator& rhs) {
        if (lhs.bus_iterator_begin == lhs.bus_iterator_end || rhs.bus_iterator_begin == rhs.bus_iterator_end) {
            return lhs.bus_iterator_begin == rhs.bus_iterator_begin;
        } else {
            return lhs.bus_iterator_begin == rhs.bus_iterator_begin && lhs.dest_iterator == rhs.dest_iterator;
        }
    }
    friend bool operator!=(const PinIterator& lhs, const PinIterator& rhs) {
        return !(lhs == rhs);
    }

private:
    TBusIterator bus_iterator_begin;
    TBusIterator bus_iterator_end;
    TDestIterator dest_iterator;

};

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

    static auto pinsBegin(const model::Scheme& scheme, const TBlockKey& id) {
        auto begin_it = scheme.busses.lower_bound({ .src = { .block_id = id } });
        return PinIterator(begin_it, scheme.busses.end(), begin_it->dests.begin());
    }

    static auto pinsEnd(const model::Scheme& scheme, const TBlockKey& id) {
        auto end_it = scheme.busses.lower_bound({ .src = { .block_id = id + 1 } });
        return PinIterator(end_it, scheme.busses.end(), end_it->dests.begin());
    }

    static TBlockKey blockKey(const model::Pin& pin) {
        return pin.block_id;
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

    for (auto it = TExtractorPolicy::pinsBegin(scheme, blockKey); it != TExtractorPolicy::pinsEnd(scheme, blockKey); ++it) {
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
