#ifndef OMVIZ_NODETYPEMANAGER_HPP
#define OMVIZ_NODETYPEMANAGER_HPP

#include <QGraphicsItem>
#include <graph_analysis/Vertex.hpp>
#include <boost/assign/list_of.hpp>

namespace omviz {
namespace node {
    typedef std::string Type;
}

class NodeTypeManager
{
public:
    typedef std::map<std::string, QGraphicsItem*> ClassVisualizationMap;

private:
    ClassVisualizationMap mClassVisualizationMap;

public:
    NodeTypeManager();
    ~NodeTypeManager();

    // Register visualization class
    // takes ownership of graphicsItem
    void registerVisualization(const node::Type& type, QGraphicsItem* graphicsItem);

    QGraphicsItem* graphicsItemByType(const node::Type& type);

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, const node::Type& type);
    QPainterPath shape(const node::Type& type);
    QRectF boundingRect(const node::Type& type);
};

} // end namespace omviz
#endif // OMVIZ_NODETYPEMANAGER_HPP
