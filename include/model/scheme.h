//
// Created by Stepan on 31.12.2021.
//

#ifndef BLOCK_ENGINE_SCHEME_H
#define BLOCK_ENGINE_SCHEME_H

#include <map>
#include <set>
#include <string>

namespace model {

    struct Block {
        int block_type_id;
        int input_count;
        int output_count;
    };

    struct Link {
        int type_id;
        int block_out_id;
        int bus_out_idx;
        int block_in_id;
        int bus_in_idx;
    };

    struct LinkLess {
        bool operator()(const Link& lhs, const Link& rhs) const {
            if (lhs.block_out_id < rhs.block_out_id)
                return true;
            if (rhs.block_out_id < lhs.block_out_id)
                return false;
            if (lhs.bus_out_idx < rhs.bus_out_idx)
                return true;
            if (rhs.bus_out_idx < lhs.bus_out_idx)
                return false;
            if (lhs.block_in_id < rhs.block_in_id)
                return true;
            if (rhs.block_in_id < lhs.block_in_id)
                return false;
            return lhs.bus_in_idx < rhs.bus_in_idx;
        }
    };

    struct Scheme {
        std::map<int, std::string> types;
        std::map<int, std::string> block_types;
        std::map<int, Block> blocks;
        std::set<Link, LinkLess> links;

        bool addType(int type_id, const std::string& type);
        std::optional<std::string> getType(int type_id);
        bool addBlockType(int block_id, const std::string& block_type);
        std::optional<std::string> getBlockType(int block_type_id);
        bool addBlock(int block_id, const Block& block);
        std::optional<Block> getBlock(int block_id);
        bool addLink(const Link& link);
    };

}

#endif //BLOCK_ENGINE_SCHEME_H
