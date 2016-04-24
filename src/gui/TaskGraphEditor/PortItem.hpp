#ifndef GRAPH_ANALYSIS_GUI_PORTITEM_HPP
#define GRAPH_ANALYSIS_GUI_PORTITEM_HPP

#include <QGraphicsItemGroup>

#include <graph_analysis/gui/GraphicsItemTypes.hpp>
#include <graph_analysis/gui/VertexItemBase.hpp>

#include <graph_analysis/task_graph/Port.hpp>

class QGraphicsSceneMouseEvent;
class QFatRact;

namespace graph_analysis
{
namespace gui
{

class GraphWidget;

class PortItem : public VertexItemBase
{
  public:
    PortItem(GraphWidget *graphWidget, graph_analysis::task_graph::Port::Ptr vertex,
             QGraphicsItem *parent);
    virtual ~PortItem();

    virtual void updateStrings() = 0;

    void setRectBrush(QBrush brush);

  protected:
    QFatRact *mpRect;
    QGraphicsTextItem *mpLabel;
    QGraphicsTextItem *mpDataType;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
