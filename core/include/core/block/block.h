#ifndef BLOCK_ENGINE_CORE_BLOCK_H
#define BLOCK_ENGINE_CORE_BLOCK_H

#include <functional>
#include <tuple>
#include <typeinfo>
#include <utility>

#include "core/connector.h"
#include "model/block.h"

namespace block_engine::core {

template<typename Tp>
class Ref {
    Tp* data;
public:

    Ref() : data(nullptr) { }

    template<typename TRef>
    Ref(TRef&& ref) : data(std::addressof(std::forward<TRef>(ref))) { }

    Tp& get() const {
        return *data;
    }

    Ref& operator=(const Ref&) = default;
};

class BlockLogicBase {
public:
    model::Block block;

    virtual ~BlockLogicBase() = default;
    virtual bool start() { return true; }
    virtual bool calc() = 0;
    virtual bool stop() { return true; }
    virtual void connectInputs(Connector&) { };
    virtual void connectOutputs(Connector&) { };
    virtual void updateSettings() { }

    void setBlock(const model::Block& block_);
};

typedef std::shared_ptr<BlockLogicBase> BlockLogicBasePtr;

template <typename Tp, typename... Args>
BlockLogicBasePtr MakeBlockLogicBasePtr(Args&&... args) {
    std::shared_ptr<BlockLogicBase> base(new Tp(std::forward<Args>(args)...));
    return base;
}

template <typename TBlock, typename ...TArgs>
auto makeBlockInitializer(const std::string& block_name, TArgs ...args) {
    return [=]() { return MakeBlockLogicBasePtr<TBlock>(args...); };
}

}

//template<
//    typename InType,
//    typename OutType
//>
//class BaseBlock : public IBlockLogic {
//public:
//    typedef InType InCollectionType;
//    typedef OutType OutCollectionType;
//
//    BaseBlock(InCollectionType& inCollection,
//              OutCollectionType& outCollection): in_collection(inCollection), out_collection(outCollection) { }
//
//    bool calc() override {
//        return calc_internal(in_collection, out_collection);
//    }
//
//    virtual bool calc_internal(const InCollectionType& input, OutCollectionType& output) = 0;
//
//private:
//    InCollectionType& in_collection;
//    OutCollectionType& out_collection;
//};
//
//template<typename ReturnType, typename... InputParamTypes>
//class SimpleSumBlock : public BaseBlock<std::tuple<InputParamTypes&...>, std::tuple<ReturnType&>> {
//public:
//    SimpleSumBlock(std::tuple<InputParamTypes&...>& inCollection, std::tuple<ReturnType&>& outCollection)
//        : BaseBlock<std::tuple<InputParamTypes&...>, std::tuple<ReturnType&>>(inCollection, outCollection) {}
//
//    bool calc_internal(const std::tuple<InputParamTypes&...> &input, std::tuple<ReturnType&> &output) override {
//        std::get<0>(output) = ReturnType();
//        std::apply([&output](auto&&... args) {
//            ((std::get<0>(output) = std::get<0>(output) + args), ...);
//        }, input);
//        return true;
//    }
//};

#endif //BLOCK_ENGINE_CORE_BLOCK_H
