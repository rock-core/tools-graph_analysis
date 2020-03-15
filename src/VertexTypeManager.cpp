#include "VertexTypeManager.hpp"

#include <boost/assign/list_of.hpp>
#include <base-logging/Logging.hpp>

namespace graph_analysis {

VertexTypeManager::VertexTypeManager()
    : base::Singleton<VertexTypeManager>()
    , AttributeManager()
{
    Vertex::Ptr vertex = make_shared<Vertex>();
    registerType(vertex);
    setDefaultType( vertex->getClassName() );

}

void VertexTypeManager::registerType(const Vertex::Ptr& vertex,
        const Vertex::PtrList& parents,
        bool throwOnAlreadyRegistered)
{
    //Create a empty structure for this type to make sure getAttributes raise if a unregistered vertex type is queried
    registerType(vertex->getClassName(), vertex, parents, throwOnAlreadyRegistered);
}

void VertexTypeManager::registerType(const vertex::Type& type,
        const Vertex::Ptr& node,
        const Vertex::PtrList& parents,
        bool throwOnAlreadyRegistered)
{
    assert(node);

    for(const Vertex::Ptr& sub : parents)
    {
        LOG_WARN_S << "Subclass of: " << sub->getClassName();
    }


    if(node->getClassName() != type)
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager: cannot"
            "register vertex of type " + type + " it seems the getClassName()"
            "function of this class is implemented wrong it returned '" +
            node->getClassName() + "'");
    }

    try {
        vertexByType(type, true);
        LOG_INFO_S << "VertexType '" + type + "' is already registered.";
    } catch(...)
    {
        LOG_INFO_S << "VertexType '" + type + "' is newly registered";
        mTypeMap[type] = node;
        mRegisteredTypes.insert(type);
        mTypeHierarchy[type] = {};
        for(const Vertex::Ptr& v : parents)
        {
            mTypeHierarchy[type].insert(v->getClassName());
        }
        activateAttributedType(type);
        return;
    }
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager::registerType: type '" + type + "' is already registered");
    }
}

Vertex::Ptr VertexTypeManager::vertexByType(const vertex::Type& type, bool throwOnDefault)
{
    TypeMap::const_iterator it = mTypeMap.find(type);
    if(it == mTypeMap.end())
    {
        LOG_DEBUG_S << "type '" + type + "' is not registered.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' is not registered");
        }
        LOG_DEBUG_S << "Using default VertexType '" << mDefaultVertexType << "'";
        return mTypeMap[mDefaultVertexType];
    }

    LOG_DEBUG_S << "VertexType '" + type + "' found.";
    return it->second;
}

void VertexTypeManager::setDefaultType(const std::string& defaultType)
{
    if( std::find(mRegisteredTypes.begin(), mRegisteredTypes.end(), defaultType) == mRegisteredTypes.end())
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager::setDefaultVertexType: type '"
                + defaultType + "' is not a registered vertex type");
    }
    mDefaultVertexType = defaultType;
}

Vertex::Ptr VertexTypeManager::createVertex(const vertex::Type& type, const std::string& label, bool throwOnMissing)
{
    Vertex::Ptr v = vertexByType(type, throwOnMissing);
    if(!v){
        throw std::invalid_argument("graph_analysis::VertexTypeManager: cannot get node for type: " + type + " and label " + label);
    }
    Vertex::Ptr clonedVertex = v->clone();
    if(v->getClassName() != clonedVertex->getClassName())
    {
        std::stringstream ss;
        ss << "graph_analysis::VertexTypeManager: cannot create cloned vertex of type " + v->getClassName() + " it seems the 'virtual Vertex* getClone() const' function of this class is implemented wrong";
        LOG_WARN_S << ss.str();
        throw std::runtime_error(ss.str());
    }

    clonedVertex->setLabel(label);
    return clonedVertex;
}

std::set<std::string> VertexTypeManager::getSupportedTypes() const
{
    return mRegisteredTypes;
}

std::vector<std::string> VertexTypeManager::getTypeHierarchy(const vertex::Type& type) const
{
    std::set<std::string> types;
    types.insert(type);

    const std::map<std::string, std::set<std::string> >::const_iterator cit = mTypeHierarchy.find(type);
    if(cit == mTypeHierarchy.end())
    {
        std::stringstream ss;
        for(const std::string& type : getSupportedTypes())
        {
            ss << type;
            ss << ", ";
        }
        throw
            std::runtime_error("graph_analysis::VertexTypeManager::getTypeHierarchy: "
                    "type '" + type + "' is not known. Registered types are: " +
                    ss.str());
    }

    const std::set<std::string>& ancestors = cit->second;
    for(const std::string& ancestor : ancestors)
    {
        std::vector<std::string> parents = getTypeHierarchy(ancestor);
        types.insert(parents.begin(), parents.end());
    }
    return std::vector<std::string>(types.begin(), types.end());
}

std::vector<Attribute> VertexTypeManager::getAttributes(const vertex::Type& type,
        bool includeLegacySupport) const
{
    std::vector<Attribute> allAttributes;
    try {
        uint32_t memberCount = 0;
        for(const std::string& classname : getTypeHierarchy(type))
        {
            for(const std::string& memberName : AttributeManager::getAttributeNames(classname) )
            {
                std::stringstream attrId;
                attrId << classname << "-attribute-" << memberCount;
                Attribute attribute(attrId.str(), memberName, classname, "STRING");
                LOG_DEBUG_S << "Adding custom vertex attribute: " << attribute.toString();
                allAttributes.push_back(attribute);

                // Serialization of vertices in early versions, did not support
                // class hierarchies
                // To allow still to read this kind of files the legacy support
                // is added
                if(includeLegacySupport && classname != type)
                {
                    std::stringstream attrId;
                    attrId << type << "-attribute-" << memberCount;
                    Attribute attribute(attrId.str(), memberName, classname, "STRING");
                    LOG_DEBUG_S << "Adding legacy support vertex attribute: " << attribute.toString();
                    allAttributes.push_back(attribute);
                }

                ++memberCount;
            }
        }
    } catch(const std::exception& e)
    {
        LOG_WARN_S << "No extra attributes can be serialized for this vertex type: '"
            << type << "': " << e.what();
    }
    return allAttributes;
}

std::vector<Attribute> VertexTypeManager::getKnownAttributes(const std::set<std::string>& classnames) const
{
    std::vector<Attribute> allAttributes;

    for(const std::string& classname : getSupportedTypes())
    {
        if(!classnames.empty())
        {
            if( classnames.count(classname) == 0)
            {
                continue;
            }
        }
        std::vector<Attribute> attributes = getAttributes(classname);
        allAttributes.insert(allAttributes.end(), attributes.begin(), attributes.end());
    }
    return allAttributes;
}

std::vector< std::pair<Attribute::Id, std::string> > VertexTypeManager::getAttributeValues(const Vertex::Ptr& vertex) const
{
    std::vector< std::pair<Attribute::Id, std::string> > attributeAssignments;

    for(const Attribute& attribute : getAttributes(vertex->getClassName()))
    {
        io::AttributeSerializationCallbacks callbacks = getAttributeSerializationCallbacks(attribute);
        std::string data = callbacks.serializeFunction(vertex);
        attributeAssignments.push_back(std::pair<Attribute::Id,std::string>(attribute.getId(),data));
    }

    return attributeAssignments;
}

} // end namespace graph_analysis
