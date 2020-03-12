#include "EdgeTypeManager.hpp"
#include <boost/assign/list_of.hpp>
#include <base-logging/Logging.hpp>

namespace graph_analysis {

EdgeTypeManager::EdgeTypeManager()
{
    Edge::Ptr edge = make_shared<Edge>();
    registerType(edge);
    setDefaultType(edge->getClassName());
}

void EdgeTypeManager::registerType(const Edge::Ptr& edge,
        const Edge::PtrList& parents,
        bool throwOnAlreadyRegistered)
{
    //Create a empty structure for this type to make sure getMembers raise if a unregistered edge type is queried
    registerType(edge->getClassName(), edge, parents, throwOnAlreadyRegistered);
}

void EdgeTypeManager::registerType(const edge::Type& type,
        const Edge::Ptr& edge,
        const Edge::PtrList& parents,
        bool throwOnAlreadyRegistered)
{
    assert(edge);

    if(edge->getClassName() != type)
    {
        throw std::runtime_error("graph_analysis::EdgeTypeManager: cannot"
                "register edge of type " + type + " it seems the getClassName()"
                "function of this class is implemented wrong it returned '" +
                edge->getClassName() + "'");
    }

    try {
        edgeByType(type, true);
        LOG_INFO_S << "EdgeType '" + type + "' is already registered.";
    } catch(...)
    {
        LOG_INFO_S << "EdgeType '" + type + "' is newly registered.";
        mTypeMap[type] = edge;
        mRegisteredTypes.insert(type);
        mTypeHierarchy[type] = {};
        for(const Edge::Ptr& e : parents)
        {
            mTypeHierarchy[type].insert(e->getClassName());
        }

        activateAttributedType(type);
        return;
    }
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::EdgeTypeManager::registerType: type '" + type + "' is already registered");
    }
}

Edge::Ptr EdgeTypeManager::edgeByType(const edge::Type& type, bool throwOnDefault)
{
    TypeMap::const_iterator it = mTypeMap.find(type);
    if(it == mTypeMap.end())
    {
        LOG_DEBUG_S << "EdgeType '" + type + "' is not registered.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::EdgeTypeManager::edgeByType: type '" + type + "' is not registered");
        }
        LOG_DEBUG_S << "Using default EdgeType '" << mDefaultType << "'";
        return mTypeMap[mDefaultType];
    }

    LOG_DEBUG_S << "EdgeType '" + type + "' found";
    return it->second;
}

void EdgeTypeManager::setDefaultType(const std::string& defaultType)
{
    if( std::find(mRegisteredTypes.begin(), mRegisteredTypes.end(), defaultType) == mRegisteredTypes.end())
    {
        throw std::runtime_error("graph_analysis::EdgeTypeManager::setDefaultType: type '"
                + defaultType + "' is not a registered edge type");
    }
    mDefaultType = defaultType;
}

Edge::Ptr EdgeTypeManager::createEdge(const edge::Type& type, const std::string& label, bool throwOnMissing)
{
    Edge::Ptr clonedEdge = edgeByType(type, throwOnMissing)->clone();
    clonedEdge->setLabel(label);
    return clonedEdge;
}

Edge::Ptr EdgeTypeManager::createEdge(const edge::Type& type, const Vertex::Ptr& source, const Vertex::Ptr& target,
        const std::string& label,
        bool throwOnMissing)
{
    Edge::Ptr clonedEdge = edgeByType(type, throwOnMissing)->clone();
    clonedEdge->setLabel(label);
    clonedEdge->setSourceVertex(source);
    clonedEdge->setTargetVertex(target);
    return clonedEdge;
}

std::set<std::string> EdgeTypeManager::getSupportedTypes() const
{
    return mRegisteredTypes;
}

std::vector<std::string> EdgeTypeManager::getTypeHierarchy(const edge::Type& type) const
{
    std::set<std::string> types;
    types.insert(type);

    const std::map<std::string, std::set<std::string> >::const_iterator cit = mTypeHierarchy.find(type);
    if(cit == mTypeHierarchy.end())
    {
        throw
            std::runtime_error("graph_analysis::EdgeTypeManager::getTypeHierarchy: "
                    "type '" + type + "' is not known. Did you register it?");
    }

    const std::set<std::string>& ancestors = cit->second;
    for(const std::string& ancestor : ancestors)
    {
        std::vector<std::string> parents = getTypeHierarchy(ancestor);
        types.insert(parents.begin(), parents.end());
    }
    return std::vector<std::string>(types.begin(), types.end());
}

std::vector<Attribute> EdgeTypeManager::getAttributes(const edge::Type& type,
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
                LOG_DEBUG_S << "Adding custom edge attribute: " << attribute.toString();
                allAttributes.push_back(attribute);

                // Serialization of edges in early versions, did not support
                // class hierarchies
                // To allow still to read this kind of files the legacy support
                // is added
                if(includeLegacySupport && classname != type)
                {
                    std::stringstream attrId;
                    attrId << type << "-attribute-" << memberCount;
                    Attribute attribute(attrId.str(), memberName, classname, "STRING");
                    LOG_DEBUG_S << "Adding legacy support edge attribute: " << attribute.toString();
                    allAttributes.push_back(attribute);
                }

                ++memberCount;
            }
        }
    } catch(const std::exception& e)
    {
        LOG_WARN_S << "No extra attributes can be serialized for this edge type: '"
            << type << "': " << e.what();
    }
    return allAttributes;
}

std::vector<Attribute> EdgeTypeManager::getKnownAttributes(const std::set<std::string>& classnames) const
{
    std::vector<Attribute> allAttributes;

    std::set<std::string> types = getSupportedTypes();
    for(std::set<std::string>::const_iterator type_it = types.begin(); type_it != types.end(); type_it++)
    {
        std::string classname = *type_it;
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

std::vector< std::pair<Attribute::Id, std::string> > EdgeTypeManager::getAttributeValues(const Edge::Ptr& edge) const
{
    std::vector< std::pair<Attribute::Id, std::string> > attributeAssignments;

    for(const Attribute& attribute : getAttributes(edge->getClassName()))
    {
        io::AttributeSerializationCallbacks callbacks = getAttributeSerializationCallbacks(attribute);
        std::string data = (edge.get()->*callbacks.serializeFunction)();

        if(!data.empty())
        {
            attributeAssignments.push_back(std::pair<Attribute::Id,std::string>(attribute.getId(), data));
        }
    }
    return attributeAssignments;
}

} // end namespace graph_analysis
