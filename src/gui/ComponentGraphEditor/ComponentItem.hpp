#ifndef GRAPH_ANALYSIS_GUI_COMPONENTITEM_HPP
#define GRAPH_ANALYSIS_GUI_COMPONENTITEM_HPP

#include <QGraphicsItemGroup>

#include <graph_analysis/gui/GraphicsItemTypes.hpp>
#include <graph_analysis/gui/VertexItemBase.hpp>

#include <graph_analysis/gui/ComponentGraphEditor/Component.hpp>

class QGraphicsSceneMouseEvent;

namespace graph_analysis
{
namespace gui
{

class InputPortItem;
class OutputPortItem;
class GraphWidget;

/* simplest possible implementation: just a box with two strings -- type and
 * label */
class ComponentItem : public VertexItemBase
{
  public:
    ComponentItem(GraphWidget* graphWidget,
                  graph_analysis::Component::Ptr vertex, QGraphicsItem* parent);
    ~ComponentItem();
    virtual int type() const { return ComponentItemType; };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = 0);
    QRectF boundingRect() const;

  private:
    QGraphicsTextItem* mpLabel;
    QGraphicsTextItem* mpClassName;
    QGraphicsRectItem* mpRect;
    QVector<InputPortItem*> mvInputPorts;
    QVector<OutputPortItem*> mvOutputPorts;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
