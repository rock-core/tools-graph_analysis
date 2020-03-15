#ifndef GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP

#include <map>
#include <set>
#include <string>
#include <typeinfo>
#include <base-logging/Singleton.hpp>

#include "Vertex.hpp"
#include "AttributeManager.hpp"

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
class VertexTypeManager : public base::Singleton<VertexTypeManager>, public AttributeManager
{
public:
    typedef std::map<vertex::Type, Vertex::Ptr> TypeMap;

private:

    /// registration map - stores the registered types, mapping them to the example vertex instances (i.e. from which new ones to be forked on request)
    TypeMap mTypeMap;
    /// registration list - maintains a complete list of all registered types
    std::set<std::string> mRegisteredTypes;
    /// the type hierarchy mapping type to parent(s)
    std::map<std::string, std::set<std::string> > mTypeHierarchy;
    /// The default vertex type
    std::string mDefaultVertexType;

    /**
     * \brief internal method for type identification
     * \param type requested vertex type
     * \param throwOnMissing flag indicating whether exceptions shall be thrown when fed with unregistered types; on false it silently picks the default type
     * \return smart pointer to the witness vertex instance of the requested type
     */
    Vertex::Ptr vertexByType(const vertex::Type& type, bool throwOnMissing = false);

protected:
    /// constructor
    VertexTypeManager();
    friend class base::Singleton<VertexTypeManager>;

public:
    /**
     * Select the default vertex type from the list of registered types
     */
    void setDefaultType(const std::string& type);

    /**
     * Get the default vertex type
     */
    const std::string& getDefaultType() const { return mDefaultVertexType; }

    template<typename T, typename P = graph_analysis::Vertex>
    void registerType(bool throwOnAlreadyRegistered = false)
    {
        Vertex::Ptr vertex = make_shared<T>();
        Vertex::PtrList parents;
        if( std::is_base_of<P,T>::value )
        {
            if(!std::is_same<graph_analysis::Vertex, P>::value)
            {
                parents.push_back( make_shared<P>() );
            }
        } else {
            throw
                std::invalid_argument("graph_analysis::VertexTypeManager::registerType:"
                        " invalid registration since '" + std::string(typeid(T).name()) + "' "
                        "is not derived from '" + std::string(typeid(P).name()) + "'");
        }
        registerType(vertex, parents, throwOnAlreadyRegistered );
        vertex->registerAttributes(this);
    }

    /**
     * Register vertex class
     * \param vertex Instance of the registered type
     * \param parents Instances of the parent types
     * \param throwOnAlreadyRegistered If true, then this function will throw
     *      std::runtime_error upon double registration
     */
    void registerType(const Vertex::Ptr& vertex,
            const Vertex::PtrList& parents = Vertex::PtrList(),
            bool throwOnAlreadyRegistered = false);

    /* Register vertex class
     * \param type Type name -- GraphElement::getClassName() of the vertex which shall be registered
     * \param vertex Instance of the registered type
     * \param parents Instances of the parent types
     * \param throwOnAlreadyRegistered If true, then this function will throw
     * \param throwOnAlreadyRegistered If true, then this function will throw
     *      std::runtime_error upon double registration
     */
    void registerType(const vertex::Type& type,
            const Vertex::Ptr& vertex,
            const Vertex::PtrList& parents = Vertex::PtrList(),
            bool throwOnAlreadyRegistered = false);

    /**
     * \brief clones a new vertex of a specified type
     * \param type the requested vertex type
     * \param label the requested vertex label
     * \param throwOnMissing Throw exception when the vertex type is unknown,
     * otherwise a default vertex will be created
     * \return smart pointer to the newly created vertex instance
     */
    Vertex::Ptr createVertex(const vertex::Type& type, const std::string& label = std::string(),
            bool throwOnMissing = false);

    /**
     * Lists the registered types
     * \return list of registered types
     */
    std::set<std::string> getSupportedTypes() const;

    /**
     * Compute the class hierarchy for a given vertex type, note that the
     * type itself will also be part of the returned list
     * \param type Vertex type for which the hierarchy should be retrieved
     * \return class hierarchy
     */
    std::vector<std::string> getTypeHierarchy(const vertex::Type& type) const;

    /**
     * Get all attributes that are associated with the registered types
     * \param classnames generate the known attributes only for the given
     * classnames
     * \see AttributeManager::getAttributes to retrieve information for an
     * individual attribute
     */
    std::vector<Attribute> getKnownAttributes(const std::set<std::string>& classnames = std::set<std::string>()) const;

    /**
     * Get all attributes that are associated with a registered type
     * \param classname retrieve attributes only for the given
     * classname
     * \see AttributeManager::getAttributeName to retrieve information for an
     * individual attribute
     */
    std::vector<Attribute> getAttributes(const vertex::Type& classname,
            bool includeLegacySupport = false) const;

    /**
     * Retrieve the attribute data for a particular vertex
     * \return value assignments for the attributes of a vertex
     */
    std::vector< std::pair<Attribute::Id, std::string> >
        getAttributeValues(const Vertex::Ptr& vertex) const;
};

} // end namespace graph_analysis
#endif /* GRAPH_ANALYSIS_VERTEX_TYPE_MANAGER_HPP */
