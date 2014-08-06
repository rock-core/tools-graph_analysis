#ifndef OMVIZ_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
#define OMVIZ_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP

#include <omviz/graphwidget/NodeTypeManager.hpp>
#include <QGraphicsItem>

namespace omviz {
namespace graphitem {

class Resource : public QGraphicsItem
{
public:
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
    virtual void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);
};

} // end namespace graphitem
} // end namespace omviz
#endif // OMVIZ_GRAPHWIDGET_GRAPHITEM_RESOURCE_HPP
