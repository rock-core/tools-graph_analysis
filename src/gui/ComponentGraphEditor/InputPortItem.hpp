#ifndef GRAPH_ANALYSIS_GUI_INPUTPORTITEM_HPP
#define GRAPH_ANALYSIS_GUI_INPUTPORTITEM_HPP

#include <QGraphicsItemGroup>

#include <graph_analysis/gui/GraphicsItemTypes.hpp>
#include <graph_analysis/gui/VertexItemBase.hpp>

#include <graph_analysis/gui/ComponentGraphEditor/InputPort.hpp>
#include <graph_analysis/gui/GraphicsItemTypes.hpp>


namespace graph_analysis
{
namespace gui
{

class GraphWidget;

/* simplest possible implementation: just a box with two strings -- type and
 * label */
class InputPortItem : public VertexItemBase
{
  public:
    InputPortItem(GraphWidget *graphWidget,
                  graph_analysis::InputPort::Ptr vertex, QGraphicsItem *parent);
    ~InputPortItem();
    virtual int type() const { return InputPortItemType; };

  private:
    QGraphicsTextItem *mpLabel;
    QGraphicsRectItem *mpRect;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
