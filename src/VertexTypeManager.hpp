#ifndef GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP

#include <map>
#include <set>
#include <string>
#include <base/Singleton.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

namespace vertex {
    // datatype for vertex type specification
    typedef std::string Type;
} // end namespace vertex

/**
 * \brief Factory for Vertex subclasses
 */
class VertexTypeManager : public base::Singleton<VertexTypeManager>
{
public:
    typedef std::map<vertex::Type, Vertex::Ptr> TypeMap;

private:
    /// registration map - stores the registered types, mapping them to the example vertex instances (i.e. from which new ones to be forked on request)
    TypeMap mTypeMap;
    /// registration list - maintains a complete list of all registered types
    std::set<std::string> mRegisteredTypes;
    /**
     * \brief internal method for type identification
     * \param type requested vertex type
     * \param throwOnDefault flag indicating whether exceptions shall be thrown when fed with unregistered types; on false it silently picks the default type
     * \return smart pointer to the witness vertex instance of the requested type
     */
    Vertex::Ptr vertexByType(const vertex::Type& type, bool throwOnDefault = false);

public:
    /// constructor
    VertexTypeManager();
    /// destructor
    ~VertexTypeManager();

    // Register vertex class
    void registerType(const vertex::Type& type, Vertex::Ptr vertex, bool throwOnAlreadyRegistered = false);
    /**
     * \brief clones a new vertex of a specified type
     * \param type the requested vertex type
     * \param label the requested vertex label
     * \return smart pointer to the newly created vertex instance
     */
    Vertex::Ptr createVertex(const vertex::Type& type, const std::string& label = std::string());

    /**
     * Lists the registered types
     * \return list of registered types
     */
    std::set<std::string> getSupportedTypes();
};

} // end namespace graph_analysis
#endif /* GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP */
