#ifndef GRAPH_ANALYSIS_ATTRIBUTE_MANAGER_HPP
#define GRAPH_ANALYSIS_ATTRIBUTE_MANAGER_HPP

#include "io/Serialization.hpp"

namespace graph_analysis {

/**
  * \brief Manage object (vertex/edge) attributes and allow them to be
  * serialized, e.g., using GEXF
  */
class AttributeManager
{
public:
    typedef std::map<std::string, io::AttributeSerializationCallbacks> AttributeSerializationCallbackMap;

    void activateAttributedType(const std::string& typeName) { (void) mRegisteredCallbacks[typeName]; }

    /**
     *  \brief register a new attribute for serialization and deserialization
     *  \param typeName the class Name (normally equals Vertex::getClassName()
     *  \param attributeName arbitrary unique name for the attribute that should added
     *  \param sF Serialization function
     *  \param dF DeSerialization function
     *  \param pF PrintFunction
     */
    void registerAttribute(const std::string& typeName,
            const std::string& attributeName,
            io::AttributeSerializationCallbacks::serialize_func_t sF,
            io::AttributeSerializationCallbacks::deserialize_func_t dF,
            io::AttributeSerializationCallbacks::print_func_t pF);

    /**
     *  \brief returns all registered members for the given vertex
     *  \param vertexTypeName the class Name (normally equals Vertex::getClassName()
     *  \return a list of members that are registered
     */
    std::vector<std::string> getAttributes(const std::string &vertexTypeName) const;

    /**
     *  \param typeName the class Name (normally equals Vertex::getClassName()
     *  \param attributeName arbitrary unique name for the attribute that should be rerieved
     *  \return the struct with all required callback functions
     */
    io::AttributeSerializationCallbacks getAttributeSerializationCallbacks(const std::string& typeName, const std::string& attributeName);

private:
    std::map<std::string, AttributeSerializationCallbackMap > mRegisteredCallbacks;

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ATTRIBUTE_MANAGER_HPP
