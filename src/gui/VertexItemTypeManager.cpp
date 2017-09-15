#include "VertexItemTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base-logging/Logging.hpp>

#include "VertexItemBase.hpp"
#include "VertexItemSimple.hpp"

namespace graph_analysis {
namespace gui {

VertexItemTypeManager::VertexItemTypeManager()
{
    registerVisualization("default", dynamic_cast<VertexItemBase*>(new VertexItemSimple()));
}

VertexItemTypeManager::~VertexItemTypeManager()
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.begin();
    for(; it != mClassVisualizationMap.end(); ++it)
    {
        delete it->second;
    }
}

void VertexItemTypeManager::registerVisualization(const vertex::Type& type, VertexItemBase* graphicsItem, bool throwOnAlreadyRegistered)
{
    try {
        graphicsItemByType(type, true);
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
    LOG_WARN_S << "type '" + type + "' is already registered.";
    if(throwOnAlreadyRegistered)
    {
        throw std::runtime_error("graph_analysis::gui::VertexItemTypeManager::registerVisualization: type '" + type + "' is already registered");
    }
}

VertexItemBase* VertexItemTypeManager::graphicsItemByType(const vertex::Type& type, bool throwOnDefault)
{
    ClassVisualizationMap::const_iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        LOG_DEBUG_S << "type '" + type + "' is not registered. Using default.";
        if(throwOnDefault)
        {
            throw std::runtime_error("graph_analysis::gui::VertexItemTypeManager::graphicsItemByType: type '" + type + "' is not registered");
        }
        return mClassVisualizationMap["default"];
    }

    LOG_DEBUG_S << "GraphicsItem (Vertex) of type '" + type + "' created.";
    return it->second;
}

VertexItemBase* VertexItemTypeManager::createItem(GraphWidget* graphWidget,
        const graph_analysis::Vertex::Ptr& vertex,
        QGraphicsItem* parent,
        const std::string& type)
{
    std::string vertexType = type;
    if(type.empty())
    {
        vertexType = vertex->getClassName();
    }
    // conditionally returning a clone of the default when type is an empty string; using type in the types map otherwise
    return graphicsItemByType(vertexType)->createNewItem(graphWidget, vertex, parent);
}

QStringList VertexItemTypeManager::getSupportedTypes() const
{
    QStringList supportedTypes;
    ClassVisualizationMap::const_iterator cit = mClassVisualizationMap.begin();
    for(; cit != mClassVisualizationMap.end(); ++cit)
    {
        supportedTypes << QString(cit->first.c_str());
    }
    return supportedTypes;
}

} // end namespace gui
} // end namespace graph_analysis
