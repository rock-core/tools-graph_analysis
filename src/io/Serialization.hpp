#ifndef GRAPH_ANALYSIS_IO_SERIALIZATION_HPP
#define GRAPH_ANALYSIS_IO_SERIALIZATION_HPP

#include <vector>
#include <functional>

#include "../GraphElement.hpp"

namespace graph_analysis {
namespace io {


class AttributeSerializationCallbackHelper : public GraphElement
{
    public:
        AttributeSerializationCallbackHelper() {}
        virtual ~AttributeSerializationCallbackHelper() {}

        std::string serialize() const { throw std::runtime_error("graph_analysis::io::AttributeSerializationCallbackHelper: serialization function not registered"); }
        void deserialize(const std::string&) { throw std::runtime_error("graph_analysis::io::AttributeSerializationCallbackHelper: deserialization function not registered"); }
        std::string print() const { throw std::runtime_error("graph_analysis::io::AttributeSerializationCallbackHelper: print function not registered"); }
};


/**
 * \class AttributeCallbacks
 * \brief Support attribute callbacks to allow custom attributes to be prepared
 * for export and import
 */
struct AttributeSerializationCallbacks
{
    typedef std::string (graph_analysis::GraphElement::*serialize_func_t)();
    typedef void (graph_analysis::GraphElement::*deserialize_func_t)(const std::string&);
    typedef std::string (graph_analysis::GraphElement::*print_func_t)();

    AttributeSerializationCallbacks()
        : serializeFunction( (serialize_func_t) &AttributeSerializationCallbackHelper::serialize)
        , deserializeFunction( (deserialize_func_t) &AttributeSerializationCallbackHelper::deserialize)
        , printFunction( (print_func_t) &AttributeSerializationCallbackHelper::print)
    {
    }

    AttributeSerializationCallbacks(
            serialize_func_t sFunc,
            deserialize_func_t dFunc,
            print_func_t pFunc
            )
        : serializeFunction(sFunc)
        , deserializeFunction(dFunc)
        , printFunction(pFunc)
    {}

    /**
     * Return the serialized attribute
     */
    std::_Mem_fn<serialize_func_t> serializeFunction;
    /**
     * deserialize the attribute
     * Takes a (serialized attribute) data string as argument to update the current instance
     */
    std::_Mem_fn<deserialize_func_t> deserializeFunction;

    /**
     * Print current attribute
     */
    std::_Mem_fn<print_func_t> printFunction;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_SERIALIZATION_HPP
