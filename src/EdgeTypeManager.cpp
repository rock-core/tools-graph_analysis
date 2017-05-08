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

std::set<std::string> EdgeTypeManager::getSupportedTypes()
{
    return mRegisteredTypes;
}

} // end namespace graph_analysis
