#include "EdgeItemTypeManager.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <boost/assign/list_of.hpp>
#include <base/Logging.hpp>
#include "graphitem/edges/Simple.hpp"
#include "layeritem/edges/Simple.hpp"

namespace graph_analysis {
namespace gui {

EdgeItemTypeManager::EdgeItemTypeManager()
{
    mClassVisualizationMap = boost::assign::map_list_of
        ("default", dynamic_cast<EdgeItem*>(new graphitem::edges::Simple()))
        (LAYER_EDGE_TYPE, dynamic_cast<EdgeItem*>(new layeritem::edges::Simple())) // layering graphical edge instance
    ;
}

EdgeItemTypeManager::~EdgeItemTypeManager()
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.begin();
    for(; it != mClassVisualizationMap.end(); ++it)
    {
        delete it->second;
    }
}

void EdgeItemTypeManager::registerVisualization(const edge::Type& type, EdgeItem* graphicsItem)
{
    try {
        graphicsItemByType(type);
        throw std::runtime_error("graph_analysis::gui::EdgeItemTypeManager::registerVisualization: type '" + type + "' is already registered");
    } catch(...)
    {
        mClassVisualizationMap[type] = graphicsItem;
    }
}

EdgeItem* EdgeItemTypeManager::graphicsItemByType(const edge::Type& type)
{
    ClassVisualizationMap::iterator it = mClassVisualizationMap.find(type);
    if(it == mClassVisualizationMap.end())
    {
        return mClassVisualizationMap["default"];
    }
    return it->second;
}

EdgeItem* EdgeItemTypeManager::createItem(GraphWidget* graphWidget, QGraphicsItem* source, QGraphicsItem* target, graph_analysis::Edge::Ptr edge, const std::string& type)
{
    // conditionally returning a clone of the default when type is an empty string; using type in the types map otherwise
    std::string edgeType = type;
    if(type.empty())
    {
        edgeType = edge->getClassName();
    }
    return graphicsItemByType(edgeType)->createNewItem(graphWidget, source, target, edge);
}

QStringList EdgeItemTypeManager::getSupportedTypes() const
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
