#ifndef BLOCK_ENGINE_BLOCK_FACTORY_H
#define BLOCK_ENGINE_BLOCK_FACTORY_H

#include <map>


namespace block_engine::gui {

class BlockFactory {
public:
    using TBlock = BlockLogicBase*;
    using TBlockFactoryMap = std::map<std::string, std::function<BlockFactory::TBlock()>>;

    explicit BlockFactory(const TBlockFactoryMap &map);

    TBlock createBlockByName(const std::string& name);

private:
    TBlockFactoryMap map;
};

BlockFactory make_block_factory();

}

#endif //BLOCK_ENGINE_BLOCK_FACTORY_H
