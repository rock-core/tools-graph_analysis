#ifndef GRAPH_ANALYSIS_EDGE_TYPE_MANAGER_HPP
#define GRAPH_ANALYSIS_EDGE_TYPE_MANAGER_HPP

#include <map>
#include <set>
#include <string>
#include <typeinfo>
#include <base-logging/Singleton.hpp>

#include "Edge.hpp"
#include "AttributeManager.hpp"

namespace graph_analysis {

class Edge;

namespace edge {
    // datatype for edge type specification
    typedef std::string Type;
} // end namespace edge

/**
 * \brief Factory for Edge subclasses
 * \details EdgeTypeManager allows to register edge classes by type
 * - which is a given string. When loading a serialized graph via GraphIO::read
 * the EdgeTypeManager allows to instanciate edges corresponding to the
 * given class type -- which has to match the type string.
 * Instanciation is done via cloning the corresponding edge instance.
 */
class EdgeTypeManager : public base::Singleton<EdgeTypeManager>, public AttributeManager
{
public:
    typedef std::map<edge::Type, Edge::Ptr> TypeMap;

private:
    /// registration map - stores the registered types, mapping them to the example edge instances (i.e. from which new ones to be forked on request)
    TypeMap mTypeMap;
    /// registration list - maintains a complete list of all registered types
    std::set<std::string> mRegisteredTypes;
    /// the type hierarchy mapping type to parent(s)
    std::map<std::string, std::set<std::string> > mTypeHierarchy;
    /// The default edge type
    std::string mDefaultType;

    typedef std::map<std::string, io::AttributeSerializationCallbacks> AttributeSerializationCallbackMap;
    std::map<std::string, AttributeSerializationCallbackMap > mRegisteredCallbacks;

    /**
     * \brief internal method for type identification
     * \param type requested edge type
     * \param throwOnDefault flag indicating whether exceptions shall be thrown when fed with unregistered types; on false it silently picks the default type
     * \return smart pointer to the witness edge instance of the requested type
     */
    Edge::Ptr edgeByType(const edge::Type& type, bool throwOnDefault = false);

protected:
    /// constructor
    EdgeTypeManager();
    friend class base::Singleton<EdgeTypeManager>;
public:

    template<typename T, typename P = graph_analysis::Edge>
    void registerType(bool throwOnAlreadyRegistered = false)
    {
        Edge::Ptr edge = make_shared<T>();
        Edge::PtrList parents;
        if( std::is_base_of<P,T>::value )
        {
            if(!std::is_same<graph_analysis::Edge, P>::value)
            {
                parents.push_back( make_shared<P>() );
            }
        } else {
            throw
                std::invalid_argument("graph_analysis::EdgeTypeManager::registerType:"
                        " invalid registration since '" + std::string(typeid(T).name()) + "' "
                        "is not derived from '" + std::string(typeid(P).name()) + "'");
        }
        registerType(edge, parents, throwOnAlreadyRegistered );
        edge->registerAttributes(this);
    }

    /**
     * Register edge class
     * \param edge Instance of the registered type
     * \param parents Instances of the parent types
     * \param throwOnAlreadyRegistered If true, then this function will throw
     *      std::runtime_error upon double registration
     */
    void registerType(const Edge::Ptr& edge,
            const Edge::PtrList& parents = Edge::PtrList(),
            bool throwOnAlreadyRegistered = false);

    /* Register edge class
     * \param type Type name -- GraphElement::getClassName() of the edge which shall be registered
     * \param edge Instance of the registered type
     * \param parents Instances of the parent types
     * \param throwOnAlreadyRegistered If true, then this function will throw
     * \param throwOnAlreadyRegistered If true, then this function will throw
     *      std::runtime_error upon double registration
     */
    void registerType(const edge::Type& type,
            const Edge::Ptr& edge,
            const Edge::PtrList& parents = Edge::PtrList(),
            bool throwOnAlreadyRegistered = false);

    /**
     * Select the default edge type from the list of registered types
     */
    void setDefaultType(const std::string& type);

    /**
     * Get the current default edge type
     */
    const std::string& getDefaultType() const { return mDefaultType; }

    /**
     * \brief clones a new edge of a specified type
     * \param type the requested edge type
     * \param label the requested edge label
     * \param throwOnMissing throw when edge type is missing
     * \return smart pointer to the newly created edge instance
     */
    Edge::Ptr createEdge(const edge::Type& type,
            const std::string& label = "",
            bool throwOnMissing = false);

    Edge::Ptr createEdge(const edge::Type& type,
            const Vertex::Ptr& source,
            const Vertex::Ptr& target,
            const std::string& label = "",
            bool throwOnMissing = false);
    /// lists the registered types
    std::set<std::string> getSupportedTypes() const;

    /**
     * Compute the class hierarchy for a given edge type, note that the
     * type itself will also be part of the returned list
     * \param type Edge type for which the hierarchy should be retrieved
     * \return class hierarchy
     */
    std::vector<std::string> getTypeHierarchy(const edge::Type& type) const;

    std::vector<Attribute> getKnownAttributes(
            const std::set<std::string>& classnames = std::set<std::string>()) const;

    /**
     * Compute the attributes of a given edge type
     * \param type Edge type
     * \return attribute list
     */
    std::vector<Attribute> getAttributes(const edge::Type& type,
            bool includeLegacySupport = false) const;

    /**
     * Get the value assignments for the attributes of an edge
     */
    std::vector< std::pair<Attribute::Id, std::string> >
        getAttributeValues(const Edge::Ptr& edge) const;
};

} // end namespace graph_analysis
#endif /* GRAPH_ANALYSIS_EDGE_TYPE_MANAGER_HPP */
