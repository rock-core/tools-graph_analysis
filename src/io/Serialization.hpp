#ifndef GRAPH_ANALYSIS_IO_SERIALIZATION_HPP
#define GRAPH_ANALYSIS_IO_SERIALIZATION_HPP

#include <vector>
#include "../GraphElement.hpp"

namespace graph_analysis {
namespace io {

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

    /**
     * Return the serialized attribute
     */
    serialize_func_t serializeFunction;
    /**
     * deserialize the attribute
     * Takes a (serialized attribute) data string as argument to update the current instance
     */
    deserialize_func_t deserializeFunction;

    /**
     * Print current attribute
     */
    print_func_t printFunction;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_SERIALIZATION_HPP
