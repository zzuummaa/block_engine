#ifndef MODERN_CPP_DESIGN_BLOCK_H
#define MODERN_CPP_DESIGN_BLOCK_H

#include <functional>
#include <tuple>
#include <typeinfo>
#include "connector.h"

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

class IBlockLogic {
public:
    virtual ~IBlockLogic() = default;
    virtual bool start() { return true; }
    virtual bool calc() = 0;
    virtual bool stop() { return true; }
    virtual void connectInputs(Connector&) = 0;
    virtual void connectOutputs(Connector&) = 0;
};

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

#endif //MODERN_CPP_DESIGN_BLOCK_H
