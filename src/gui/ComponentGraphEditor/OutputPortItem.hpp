#ifndef GRAPH_ANALYSIS_GUI_OUTPUTPORTITEM_HPP
#define GRAPH_ANALYSIS_GUI_OUTPUTPORTITEM_HPP

#include <QGraphicsItemGroup>

#include <graph_analysis/gui/GraphicsItemTypes.hpp>
#include <graph_analysis/gui/VertexItemBase.hpp>

#include <graph_analysis/gui/ComponentGraphEditor/OutputPort.hpp>
#include <graph_analysis/gui/GraphicsItemTypes.hpp>


namespace graph_analysis
{
namespace gui
{

class GraphWidget;

/* simplest possible implementation: just a box with two strings -- type and
 * label */
class OutputPortItem : public VertexItemBase
{
  public:
    OutputPortItem(GraphWidget *graphWidget,
                  graph_analysis::OutputPort::Ptr vertex, QGraphicsItem *parent);
    ~OutputPortItem();
    virtual int type() const { return OutputPortItemType; };

  private:
    QGraphicsTextItem *mpLabel;
    QGraphicsRectItem *mpRect;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
