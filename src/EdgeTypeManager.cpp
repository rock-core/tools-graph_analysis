#include "EdgeTypeManager.hpp"
#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {

EdgeTypeManager::EdgeTypeManager()
{
    // registering known implemented class-types
    mTypeMap = boost::assign::map_list_of
        (std::string("default"),                         Edge::Ptr (new Edge()))
        ;

    // initializing the list of default registered types (non-repeatingly, non-verbously)
    TypeMap::iterator it = mTypeMap.begin();
    for(; mTypeMap.end() != it; ++it)
    {
        mRegisteredTypes.insert(it->first);
    }
}

EdgeTypeManager::~EdgeTypeManager()
{
}

void EdgeTypeManager::registerType(const edge::Type& type, Edge::Ptr edge, bool throwOnAlreadyRegistered)
{
    try {
        edgeByType(type, true);
    } catch(...)
    {
        mTypeMap[type] = edge;
        mRegisteredTypes.insert(type);
    }
    LOG_WARN_S << "graph_analysis::EdgeTypeManager::registerType: type '" + type + "' is already registered.";
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
        LOG_DEBUG_S << "graph_analysis::EdgeTypeManager::edgeByType: type '" + type + "' is not registered. Using default type 'default'.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::EdgeTypeManager::edgeByType: type '" + type + "' is not registered");
        }
        return mTypeMap["default"];
    }

    LOG_DEBUG_S << "graph_analysis::EdgeTypeManager::edgeByType: type '" + type + "' registered.";
    return it->second;
}

Edge::Ptr EdgeTypeManager::createEdge(const edge::Type& type, const std::string& label)
{
    Edge::Ptr clonedEdge = edgeByType(type)->clone();
    clonedEdge->setLabel(label);
    return clonedEdge;
}

std::set<std::string> EdgeTypeManager::getSupportedTypes()
{
    return mRegisteredTypes;
}

} // end namespace graph_analysis
