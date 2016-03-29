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

/**
 * simplest possible implementation: just a box with two strings -- type and label
 *
 */
class OutputPortItem : public PortItem
{
public:
    OutputPortItem(GraphWidget* graphWidget,
                   graph_analysis::OutputPort::Ptr vertex,
                   QGraphicsItem* parent);
    virtual ~OutputPortItem();
    virtual int type() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = NULL);
    QRectF boundingRect() const;

    /**
     * our connector is the middle of our right-side.
     */
    QPointF getConnector() const
    {
        return mapToScene(QLineF(boundingRect().topRight(),
                                 boundingRect().bottomRight()).pointAt(0.5));
    };

    void updateStrings();

protected:
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
};

} // end namespace gui
} // end namespace graph_analysis
#endif
