#include "EdgeTypeManager.hpp"
#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {

EdgeTypeManager::EdgeTypeManager()
{
    // registering known implemented class-types
    registerType("default", Edge::Ptr (new Edge()));
}

EdgeTypeManager::~EdgeTypeManager()
{
}

void EdgeTypeManager::registerType(const edge::Type& type, Edge::Ptr edge, bool throwOnAlreadyRegistered)
{
    try {
        edgeByType(type, true);
        LOG_INFO_S << "EdgeType '" + type + "' is already registered.";
    } catch(...)
    {
        LOG_INFO_S << "EdgeType '" + type + "' is newly registered.";
        mTypeMap[type] = edge;
        mRegisteredTypes.insert(type);
        return;
    }
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::EdgeTypeManager::registerType: type '" + type + "' is already registered");
    }
}

Edge::Ptr EdgeTypeManager::edgeByType(const edge::Type& type, bool throwOnDefault)
{
    TypeMap::iterator it = mTypeMap.find(type);
    if(it == mTypeMap.end())
    {
        LOG_DEBUG_S << "EdgeType '" + type + "' is not registered.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::EdgeTypeManager::edgeByType: type '" + type + "' is not registered");
        }
        LOG_DEBUG_S << "Using default EdgeType 'default'.";
        return mTypeMap["default"];
    }

    LOG_DEBUG_S << "EdgeType '" + type + "' found";
    return it->second;
}

Edge::Ptr EdgeTypeManager::createEdge(const edge::Type& type, const std::string& label)
{
    Edge::Ptr clonedEdge = edgeByType(type)->clone();
    clonedEdge->setLabel(label);
    return clonedEdge;
}

Edge::Ptr EdgeTypeManager::createEdge(const edge::Type& type, const Vertex::Ptr& source, const Vertex::Ptr& target, const std::string& label)
{
    Edge::Ptr clonedEdge = edgeByType(type)->clone();
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
