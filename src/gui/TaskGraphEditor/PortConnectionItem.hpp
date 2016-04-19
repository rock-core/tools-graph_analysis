#ifndef GRAPH_ANALYSIS_GUI_PORTCONNECTIONITEM_HPP
#define GRAPH_ANALYSIS_GUI_PORTCONNECTIONITEM_HPP

#include <QGraphicsItem>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
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
    PortConnectionItem(GraphWidget* graphWidget,
                       graph_analysis::task_graph::PortConnection::Ptr conn,
                       QGraphicsItem* parent);

    virtual ~PortConnectionItem();
    virtual int type() const;

protected:
    /**
     * Needed for selecting and deleting a Task
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
};

} // end namespace gui
} // end namespace graph_analysis
#endif
