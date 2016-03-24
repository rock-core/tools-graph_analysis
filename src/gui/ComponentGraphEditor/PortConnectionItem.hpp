#ifndef GRAPH_ANALYSIS_GUI_PORTCONNECTIONITEM_HPP
#define GRAPH_ANALYSIS_GUI_PORTCONNECTIONITEM_HPP

#include <QGraphicsItem>
#include <graph_analysis/Edge.hpp>

#include "PortConnection.hpp"

#include <graph_analysis/gui/GraphicsItemTypes.hpp>
#include <graph_analysis/gui/EdgeItemBase.hpp>

namespace graph_analysis
{
namespace gui
{

class GraphWidget;

class PortConnectionItem : public EdgeItemSimple
{
  public:
    PortConnectionItem(GraphWidget *graphWidget,
                       graph_analysis::PortConnection::Ptr conn,
                       VertexItemBase *source, VertexItemBase *target,
                       QGraphicsItem *parent);

    virtual ~PortConnectionItem();

    virtual int type() const { return PortConnectionItemType; };

};

} // end namespace gui
} // end namespace graph_analysis
#endif
