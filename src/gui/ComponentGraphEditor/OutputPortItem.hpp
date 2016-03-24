#ifndef GRAPH_ANALYSIS_GUI_OUTPUTPORTITEM_HPP
#define GRAPH_ANALYSIS_GUI_OUTPUTPORTITEM_HPP

#include <QGraphicsItemGroup>

#include <graph_analysis/gui/GraphicsItemTypes.hpp>

#include <graph_analysis/gui/ComponentGraphEditor/OutputPort.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/PortItem.hpp>
#include <graph_analysis/gui/GraphicsItemTypes.hpp>

namespace graph_analysis
{
namespace gui
{

class GraphWidget;

/* simplest possible implementation: just a box with two strings -- type and
 * label */
class OutputPortItem : public PortItem
{
  public:
    OutputPortItem(GraphWidget *graphWidget,
                  graph_analysis::OutputPort::Ptr vertex, QGraphicsItem *parent);
    virtual ~OutputPortItem();
    virtual int type() const { return OutputPortItemType; };

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option,
               QWidget *widget = 0);
    QRectF boundingRect() const;

    virtual QPointF getConnector() const
    {
        return mapToScene(QLineF(boundingRect().topLeft(), boundingRect().bottomLeft())
            .pointAt(0.5));
    };

    void updateStrings();

  protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

} // end namespace gui
} // end namespace graph_analysis
#endif
