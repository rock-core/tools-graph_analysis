#include "EdgeTypeManager.hpp"
#include <boost/assign/list_of.hpp>
#include <base-logging/Logging.hpp>

namespace graph_analysis {

EdgeTypeManager::EdgeTypeManager()
{
    Edge::Ptr edge(new Edge());
    registerType(edge);
    setDefaultType(edge->getClassName());
}

void EdgeTypeManager::registerType(const Edge::Ptr& edge, bool throwOnAlreadyRegistered)
{
    //Create a empty structure for this type to make sure getMembers raise if a unregistered edge type is queried
    registerType(edge->getClassName(), edge, throwOnAlreadyRegistered);
}

void EdgeTypeManager::registerType(const edge::Type& type, const Edge::Ptr& edge, bool throwOnAlreadyRegistered)
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

        uint32_t memberCount = 0;
        std::vector<std::string> attributes = AttributeManager::getAttributes(classname);
        std::vector<std::string>::const_iterator attributesIt = attributes.begin();
        for(; attributesIt != attributes.end(); attributesIt++)
        {
            std::stringstream attrId;
            attrId << *type_it << "-attribute-" << memberCount++;
            LOG_DEBUG_S << "Adding custom edge attribute: id: " << attrId.str() << ", title: " << *attributesIt << ", type: STRING";
            allAttributes.push_back(Attribute(attrId.str(), *attributesIt, "STRING"));
        }
    }
    return allAttributes;
}

std::vector< std::pair<Attribute::Id, std::string> > EdgeTypeManager::getAttributeValues(const Edge::Ptr& edge) const
{
    std::vector< std::pair<Attribute::Id, std::string> > attributeAssignments;

    std::vector<std::string> attributes = AttributeManager::getAttributes(edge->getClassName());
    uint32_t memberCount = 0;
    std::vector<std::string>::const_iterator attributesIt = attributes.begin();
    for(; attributesIt != attributes.end(); ++attributesIt)
    {
        std::stringstream attrId;
        attrId << edge->getClassName() << "-attribute-" << memberCount++;
        io::AttributeSerializationCallbacks callbacks = getAttributeSerializationCallbacks(edge->getClassName(),*attributesIt);
        std::string data = (edge.get()->*callbacks.serializeFunction)();

        if(!data.empty())
        {
            attributeAssignments.push_back(std::pair<Attribute::Id,std::string>(attrId.str(), data));
        }
    }
    return attributeAssignments;
}

} // end namespace graph_analysis
