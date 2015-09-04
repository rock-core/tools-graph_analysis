#include "EdgeTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>
#include "graphitem/edges/Simple.hpp"
#include "layeritem/edges/Simple.hpp"

namespace graph_analysis {
namespace gui {

EdgeTypeManager::EdgeTypeManager()
{
    mClassVisualizationMap = boost::assign::map_list_of
        ("default", dynamic_cast<EdgeItem*>(new graphitem::edges::Simple()))
        (LAYER_EDGE_TYPE, dynamic_cast<EdgeItem*>(new layeritem::edges::Simple())) // layering graphical edge instance
    ;
}

EdgeTypeManager::~EdgeTypeManager()
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.begin();
    for(; it != mClassVisualizationMap.end(); ++it)
    {
        delete it->second;
    }
}

void EdgeTypeManager::registerVisualization(const edge::Type& type, EdgeItem* graphicsItem)
{
    try {
        graphicsItemByType(type);
        throw std::runtime_error("graph_analysis::gui::EdgeTypeManager::registerVisualization: type '" + type + "' is already registered");
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
}

EdgeItem* EdgeTypeManager::graphicsItemByType(const edge::Type& type)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        return mClassVisualizationMap["default"];
    }
    return it->second;
}

EdgeItem* EdgeTypeManager::createItem(GraphWidget* graphWidget, NodeItem* sourceNode, NodeItem* targetNode, graph_analysis::Edge::Ptr edge, const std::string& type)
{
    // conditionally returning a clone of the default when type is an empty string; using type in the types map otherwise
    return (type.empty() ? graphicsItemByType(edge->getClassName()) : graphicsItemByType(type))->createNewItem(graphWidget, sourceNode, targetNode, edge);
}

QStringList EdgeTypeManager::getSupportedTypes() const
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
