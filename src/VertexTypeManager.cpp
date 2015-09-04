#include "VertexTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>

#include <graph_analysis/PortVertex.hpp>
#include <graph_analysis/InputPortVertex.hpp>
#include <graph_analysis/OutputPortVertex.hpp>
#include <graph_analysis/PropertyVertex.hpp>
#include <graph_analysis/OperationVertex.hpp>
#include <graph_analysis/ClusterVertex.hpp>

namespace graph_analysis {

VertexTypeManager::VertexTypeManager()
{
    // registering known implemented class-types
    mTypeMap = boost::assign::map_list_of
        (std::string("default"),       Vertex::Ptr (new Vertex()))
        (std::string("port"),       Vertex::Ptr ((Vertex *) (new PortVertex())))
        (graph_analysis::PortVertex::vertexType(),          Vertex::Ptr ((Vertex *) (new PortVertex())))
        (std::string("inputport"),  Vertex::Ptr ((Vertex *) (new InputPortVertex())))
        (graph_analysis::InputPortVertex::vertexType(),     Vertex::Ptr ((Vertex *) (new InputPortVertex())))
        (std::string("outputport"), Vertex::Ptr ((Vertex *) (new OutputPortVertex())))
        (graph_analysis::OutputPortVertex::vertexType(),    Vertex::Ptr ((Vertex *) (new OutputPortVertex())))
        (std::string("cluster"),    Vertex::Ptr ((Vertex *) (new ClusterVertex())))
        (graph_analysis::ClusterVertex::vertexType(),       Vertex::Ptr ((Vertex *) (new ClusterVertex())))
        (std::string("property"),   Vertex::Ptr ((Vertex *) (new PropertyVertex())))
        (graph_analysis::PropertyVertex::vertexType(),      Vertex::Ptr ((Vertex *) (new PropertyVertex())))
        (std::string("operation"),  Vertex::Ptr ((Vertex *) (new OperationVertex())))
        (graph_analysis::OperationVertex::vertexType(),     Vertex::Ptr ((Vertex *) (new OperationVertex())))
        ;
    // initializing the list of registered types (non-repeatingly, non-verbously)
    mRegisteredTypes.clear();
    TypeMap::iterator it = mTypeMap.begin();
    for(; mTypeMap.end() != it; ++it)
    {
        if("graph_analysis::" != it->first.substr(0, std::string("graph_analysis::").length())) // filters out verbose (&& duplicate) types
        {
            mRegisteredTypes.insert(it->first);
        }
    }
}

VertexTypeManager::~VertexTypeManager()
{
}

void VertexTypeManager::registerType(const vertex::Type& type, Vertex::Ptr node, bool throwOnAlreadyRegistered)
{
    try {
        vertexByType(type, true);
    } catch(...)
    {
        mTypeMap[type] = node;
        mRegisteredTypes.insert(type);
    }
    LOG_WARN_S << "graph_analysis::VertexTypeManager::registerType: type '" + type + "' is already registered.";
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager::registerType: type '" + type + "' is already registered");
    }
}

Vertex::Ptr VertexTypeManager::vertexByType(const vertex::Type& type, bool throwOnDefault)
{
    TypeMap::iterator it = mTypeMap.find(type);
    if(it == mTypeMap.end())
    {
        LOG_DEBUG_S << "graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' is not registered. Using default type 'default'.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' is not registered");
        }
        return mTypeMap["default"];
    }

    LOG_DEBUG_S << "graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' registered.";
    return it->second;
}

Vertex::Ptr VertexTypeManager::createVertex(const vertex::Type& type, const std::string& label)
{
    Vertex::Ptr clonedVertex = vertexByType(type)->clone();
    clonedVertex->setLabel(label);
    return clonedVertex;
}

std::set<std::string> VertexTypeManager::getSupportedTypes()
{
    return mRegisteredTypes;
}

} // end namespace graph_analysis
