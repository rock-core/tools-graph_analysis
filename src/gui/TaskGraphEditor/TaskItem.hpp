#ifndef GRAPH_ANALYSIS_GUI_TASKITEM_HPP
#define GRAPH_ANALYSIS_GUI_TASKITEM_HPP

#include <QGraphicsItemGroup>

#include <graph_analysis/gui/GraphicsItemTypes.hpp>
#include <graph_analysis/gui/VertexItemBase.hpp>

#include <graph_analysis/task_graph/Task.hpp>

class QGraphicsSceneMouseEvent;
class QFatRact;

namespace graph_analysis
{
namespace gui
{

class InputPortItem;
class OutputPortItem;
class GraphWidget;

/* simplest possible implementation: just a box with two strings -- type and
 * label */
class TaskItem : public VertexItemBase
{
public:
    TaskItem(GraphWidget* graphWidget,
             graph_analysis::task_graph::Task::Ptr vertex,
             QGraphicsItem* parent);
    ~TaskItem();
    virtual int type() const;

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget = NULL);
    QRectF boundingRect() const;

protected:

    /**
     * Needed for selecting and deleting a Task
     */
    virtual void mousePressEvent(QGraphicsSceneMouseEvent * event);

private:
    QGraphicsTextItem* mpLabel;
    QGraphicsTextItem* mpTemplateLabel;
    QFatRact* mpRect;
    QVector<InputPortItem*> mvInputPorts;
    QVector<OutputPortItem*> mvOutputPorts;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
