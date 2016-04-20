#include "InputPortItem.hpp"

#include <QPen>
#include <QGraphicsSceneMoveEvent>
#include <QDebug>

#include <base/Logging.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>
#include <graph_analysis/task_graph/InputPort.hpp>
#include <graph_analysis/task_graph/OutputPort.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
InputPortItem::InputPortItem(GraphWidget *graphWidget,
                             graph_analysis::task_graph::InputPort::Ptr vertex,
                             QGraphicsItem *parent)
    : PortItem(graphWidget, vertex, parent)
{
    updateStrings();
    setAcceptDrops(true);
}

InputPortItem::~InputPortItem()
{
}

void InputPortItem::updateStrings()
{
    mpLabel->setPlainText(QString("in: ") +
                          QString(mpVertex->getLabel().c_str()));
    mpRect->setRect(mpLabel->boundingRect());
}

void InputPortItem::paint(QPainter *painter,
                           const QStyleOptionGraphicsItem *option,
                           QWidget *widget)
{
}

QRectF InputPortItem::boundingRect() const
{
    return childrenBoundingRect();
}

void InputPortItem::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    // by default, this is not accepted.
    event->setAccepted(false);
    // checking that the vertex stored in the mimetype is different from ours
    // -- this prevents self-drag'n drops
    const EdgeMimeData* pMimeData =
        dynamic_cast<const EdgeMimeData*>(event->mimeData());
    if(pMimeData)
    {
        // check that the to ports are not part of the same task
        graph_analysis::task_graph::OutputPort::Ptr oPort =
            dynamic_pointer_cast<graph_analysis::task_graph::OutputPort>(pMimeData->mpSourceVertex);
        graph_analysis::task_graph::InputPort::Ptr iPort =
            dynamic_pointer_cast<graph_analysis::task_graph::InputPort>(getVertex());

        if(oPort->getTask(getGraph()) == iPort->getTask(getGraph()))
        {
            return;
        }
        else if(oPort->isOwnDataTypeSameAs(getGraph(), iPort))
        {
            return;
        }
        else if(oPort->isConnected(getGraph()))
        {
            return;
        }
        else if(iPort->isConnected(getGraph()))
        {
            return;
        }
        else
        {
            // after this, the mouse icon will change and the drag-object will
            // be active
            event->setAccepted(true);
            mpRect->setBrush(Qt::green);
        }
    }
    else
    {
        LOG_ERROR_S << "unexpected pointer type for mimedata?";
    }
}

void InputPortItem::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    mpRect->setBrush(Qt::NoBrush);
}

void InputPortItem::dropEvent(QGraphicsSceneDragDropEvent* event)
{
    // the other side's vertex pointer is stored inside the mimedata. we can
    // check that we did not get dropped on ourselfes for example.
    const EdgeMimeData* pMimeData =
        dynamic_cast<const EdgeMimeData*>(event->mimeData());
    if(pMimeData)
    {
        if(!pMimeData->sourceVertexIsSameAs(getVertex()))
        {
            // we are ok with the drop. now store this Items vertex in the
            // reference given in the mimedata, so that the originating side
            // can actuall ycreate and insert the new Egde.
            pMimeData->mpTargetVertex = getVertex();
            event->acceptProposedAction();
            mpRect->setBrush(Qt::NoBrush);
        }
        else
        {
            LOG_INFO_S << "source and target are the same?";
            return;
        }
    }
    else
    {
        LOG_ERROR_S << "unexpected pointer type for mimedata?";
    }
}

} // end namespace gui
} // end namespace graph_analysis
