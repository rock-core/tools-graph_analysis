#ifndef GRAPH_ANALYSIS_GUI_PORTITEM_HPP
#define GRAPH_ANALYSIS_GUI_PORTITEM_HPP

#include <QGraphicsItemGroup>

#include <graph_analysis/gui/GraphicsItemTypes.hpp>
#include <graph_analysis/gui/VertexItemBase.hpp>

#include <graph_analysis/gui/ComponentGraphEditor/Port.hpp>

class QGraphicsSceneMouseEvent;

namespace graph_analysis
{
namespace gui
{

class GraphWidget;

class PortItem : public VertexItemBase
{
  public:
    PortItem(GraphWidget *graphWidget, graph_analysis::Port::Ptr vertex,
             QGraphicsItem *parent);
    virtual ~PortItem();

    virtual void updateStrings() = 0;

  protected:
    QGraphicsRectItem *mpRect;
    QGraphicsTextItem *mpLabel;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
