#ifndef GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP

#include <map>
#include <set>
#include <list>
#include <string>
#include <base/Singleton.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

namespace vertex {
    // datatype for vertex type specification
    typedef std::string Type;
} // end namespace vertex

/**
 * \class VertexTypeManager
 * \brief Factory for Vertex subclasses
 * \details VertexTypeManager allows to register vertex classes by type
 * - which is a given string. When loading a serialized graph via GraphIO::read
 * the VertexTypeManager allows to instanciate vertices corresponding to the
 * given class type -- which has to match the type string.
 * Instanciation is done via cloning the corresponding vertex instance.
 */
class VertexTypeManager : public base::Singleton<VertexTypeManager>
{
public:
    typedef std::string (graph_analysis::Vertex::*serialize_func_t)();
    typedef std::string (graph_analysis::Vertex::*print_func_t)();
    typedef void (graph_analysis::Vertex::*deserialize_func_t)(const std::string&);

    /// MemberCallbacks -- currently only io-related callbacks
    struct MemberCallbacks
    {
        serialize_func_t serializeFunction;
        deserialize_func_t deserializeFunction;

        print_func_t printFunction;
    };

    typedef std::map<vertex::Type, Vertex::Ptr> TypeMap;

private:

    /// registration map - stores the registered types, mapping them to the example vertex instances (i.e. from which new ones to be forked on request)
    TypeMap mTypeMap;
    /// registration list - maintains a complete list of all registered types
    std::set<std::string> mRegisteredTypes;

    std::map<std::string, std::map<std::string, MemberCallbacks> > mRegisteredCallbacks;
    /**
     * \brief internal method for type identification
     * \param type requested vertex type
     * \param throwOnDefault flag indicating whether exceptions shall be thrown when fed with unregistered types; on false it silently picks the default type
     * \return smart pointer to the witness vertex instance of the requested type
     */
    Vertex::Ptr vertexByType(const vertex::Type& type, bool throwOnDefault = false);

protected:
    /// constructor
    VertexTypeManager();
    friend class base::Singleton<VertexTypeManager>;
public:

    // Register vertex class
    void registerType(Vertex::Ptr vertex, bool throwOnAlreadyRegistered = false);

    // Register vertex class
    void registerType(const vertex::Type& type, Vertex::Ptr vertex, bool throwOnAlreadyRegistered = false);

    /**
     *  \brief register a new attribute for serialization and deserialization
     *  \param typeName the class Name (normally equals Vertex::getClassName()
     *  \param attributeName arbitrary unique name for the attribute that should added
     *  \param sF Serialization function
     *  \param dF DeSerialization function
     *  \param pF PrintFunction
     */
    void registerAttribute(const std::string &typeName, const std::string &attributeName, serialize_func_t sF, deserialize_func_t dsF, print_func_t pF);

    /**
     *  \brief returns all registeres member for the given vertex
     *  \param typeName the class Name (normally equals Vertex::getClassName()
     *  \return a list of members that are registered
     */
    std::list<std::string> getMembers(const std::string &typeName);

    /**
     *  \param typeName the class Name (normally equals Vertex::getClassName()
     *  \param attributeName arbitrary unique name for the attribute that should added
     *  \return the struct with all callback function
     */
    MemberCallbacks getMemberCallbacks(const std::string &typeName, const std::string &attributeName);

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
