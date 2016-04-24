#include "InputPortItem.hpp"

#include <QPen>
#include <QGraphicsSceneMoveEvent>
#include <QDebug>

#include <base/Logging.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>
#include <graph_analysis/task_graph/InputPort.hpp>
#include <graph_analysis/task_graph/OutputPort.hpp>
#include <graph_analysis/task_graph/DataType.hpp>
#include <graph_analysis/gui/QFatRact.hpp>

namespace graph_analysis {
namespace gui {

// kiss:
InputPortItem::InputPortItem(GraphWidget* graphWidget,
                             graph_analysis::task_graph::InputPort::Ptr vertex,
                             QGraphicsItem* parent)
    : PortItem(graphWidget, vertex, parent)
    , lastRectBrush(Qt::NoBrush)
{
    updateStrings();
    setAcceptDrops(true);
}

InputPortItem::~InputPortItem()
{
}

void InputPortItem::updateStrings()
{
    // carefull, the DataType can be invalid, during graph-buildup for example
    graph_analysis::task_graph::DataType::Ptr data =
        dynamic_pointer_cast<graph_analysis::task_graph::Port>(
            getVertex())->getDataType(getGraph());

    mpLabel->setPlainText(QString("in: ") +
                          QString(mpVertex->getLabel().c_str()));

    if(!data)
    {
        mpDataType->setPlainText("N/A");
        mpDataType->setDefaultTextColor(Qt::red);
    }
    else
    {
        mpDataType->setPlainText(QString::fromStdString(data->toString()));
        mpDataType->setDefaultTextColor(Qt::gray);
    }

    mpDataType->setPos(mpLabel->pos() +
                       QPoint(0, mpLabel->boundingRect().height()));

    mpRect->setRect(childrenBoundingRect());
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
            LOG_INFO_S << "No drag-accept, Ports are part of the same Task";
            return;
        }
        else if(!oPort->isOwnDataTypeSameAs(getGraph(), iPort))
        {
            LOG_INFO_S << "No drag-accept, DataType does not match";
            return;
        }
        else if(oPort->isConnected(getGraph()))
        {
            LOG_INFO_S << "No drag-accept, oPort is already connected";
            return;
        }
        else if(iPort->isConnected(getGraph()))
        {
            LOG_INFO_S << "No drag-accept, iPort is already connected";
            return;
        }
        else
        {
            // after this, the mouse icon will change and the drag-object will
            // be active
            event->setAccepted(true);
            lastRectBrush = mpRect->brush();
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
    mpRect->setBrush(lastRectBrush);
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
            mpRect->setBrush(lastRectBrush);
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
