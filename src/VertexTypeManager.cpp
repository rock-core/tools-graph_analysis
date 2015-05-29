#include "VertexTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {

VertexTypeManager::VertexTypeManager()
{
    mClassVisualizationMap = boost::assign::map_list_of
        ("default", Vertex::Ptr (            new      Vertex()))
        ("base",    Vertex::Ptr ((Vertex *) (new BaseVertex())))
        ;
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
        mClassVisualizationMap[type] = node;
    }
    LOG_WARN_S << "graph_analysis::VertexTypeManager::registerType: type '" + type + "' is already registered.";
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::VertexTypeManager::registerType: type '" + type + "' is already registered");
    }
}

Vertex::Ptr VertexTypeManager::vertexByType(const vertex::Type& type, bool throwOnDefault)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        LOG_DEBUG_S << "graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' is not registered. Using default.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' is not registered");
        }
        return mClassVisualizationMap["default"];
    }

    LOG_DEBUG_S << "graph_analysis::VertexTypeManager::vertexByType: type '" + type + "' registered.";
    return it->second;
}

Vertex::Ptr VertexTypeManager::createVertex(vertex::Type type)
{
    return vertexByType(type)->clone();
}

} // end namespace graph_analysis
