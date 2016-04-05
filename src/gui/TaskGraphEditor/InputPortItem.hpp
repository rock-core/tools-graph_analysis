#ifndef GRAPH_ANALYSIS_GUI_INPUTPORTITEM_HPP
#define GRAPH_ANALYSIS_GUI_INPUTPORTITEM_HPP

#include <QGraphicsItemGroup>

#include <graph_analysis/gui/GraphicsItemTypes.hpp>
#include <graph_analysis/task_graph/InputPort.hpp>
#include <graph_analysis/gui/GraphicsItemTypes.hpp>

#include "PortItem.hpp"

namespace graph_analysis
{
namespace gui
{

class GraphWidget;

/* simplest possible implementation: just a box with two strings -- type and
 * label */
class InputPortItem : public PortItem
{
public:
    InputPortItem(GraphWidget* graphWidget,
                  graph_analysis::task_graph::InputPort::Ptr vertex, QGraphicsItem* parent);
    virtual ~InputPortItem();
    virtual int type() const
    {
        return InputPortItemType;
    };

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = NULL);
    QRectF boundingRect() const;

    void updateStrings();

    virtual QPointF getConnector() const
    {
        return mapToScene(QLineF(boundingRect().topLeft(),
                                 boundingRect().bottomLeft()).pointAt(0.5));
    };

protected:

    void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event);
    void dropEvent(QGraphicsSceneDragDropEvent* event);

protected:
};

} // end namespace gui
} // end namespace graph_analysis
#endif
