#include "OutputPortItem.hpp"

#include <graph_analysis/gui/ComponentGraphEditor/InputPort.hpp>
#include <graph_analysis/gui/BaseGraphView/AddEdgeDialog.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <QPen>
#include <QGraphicsSceneMoveEvent>
#include <QDrag>

#include <base/Logging.hpp>

namespace graph_analysis
{
namespace gui
{

// kiss:
OutputPortItem::OutputPortItem(GraphWidget* graphWidget,
                               graph_analysis::OutputPort::Ptr vertex,
                               QGraphicsItem* parent)
    : PortItem(graphWidget, vertex, parent)
{
    updateStrings();
    setFlag(ItemIsSelectable);
}

OutputPortItem::~OutputPortItem()
{
}

int OutputPortItem::type() const
{
    return OutputPortItemType;
}

void OutputPortItem::updateStrings()
{
    mpLabel->setPlainText(QString("out: ") +
                          QString(mpVertex->getLabel().c_str()));
    mpRect->setRect(mpLabel->boundingRect());
}

void OutputPortItem::paint(QPainter* painter,
                           const QStyleOptionGraphicsItem* option,
                           QWidget* widget)
{
}

QRectF OutputPortItem::boundingRect() const
{
    return childrenBoundingRect();
}

// drag'n drop!

// this function is called as long as it is not "accepted" (by calling the
// relevant function.
void OutputPortItem::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    // by default, this is not accepted.
    event->setAccepted(false);
    // checking that the vertex stored in the mimetype is different from ours
    // -- this prevents self-drag'n drops
    const EdgeMimeData* pMimeData =
        dynamic_cast<const EdgeMimeData*>(event->mimeData());
    if(pMimeData)
    {
        // check that the to ports are not part of the same component
        graph_analysis::OutputPort::Ptr oPort =
            dynamic_pointer_cast<OutputPort>(pMimeData->mpSourceVertex);
        graph_analysis::InputPort::Ptr iPort =
            dynamic_pointer_cast<InputPort>(pMimeData->mpTargetVertex);

        if(oPort->getComponent(getGraph()) == iPort->getComponent(getGraph()))
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

void OutputPortItem::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    mpRect->setBrush(Qt::NoBrush);
}

void OutputPortItem::dropEvent(QGraphicsSceneDragDropEvent* event)
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

void OutputPortItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
{
    Vertex::Ptr sourceVertex = getVertex();
    Vertex::Ptr targetVertex = sourceVertex;

    QDrag* drag = new QDrag(getGraphWidget());
    // stores reference to the two vertices, so that the receiving side
    // can do error-checking and store its vertex as target on success.
    EdgeMimeData* mimeData = new EdgeMimeData(sourceVertex, targetVertex);

    drag->setMimeData(mimeData);

    // when this returns, the user finished its drag-operation
    Qt::DropAction dropAction = drag->exec();

    if(dropAction == Qt::MoveAction)
    {
        // check that the targetVertex got updated
        if(!targetVertex)
        {
            LOG_ERROR_S
            << "could not find a target vertex after dropEvent";
            return;
        }
        AddEdgeDialog dialog;
        dialog.exec();
        if(dialog.result() == QDialog::Accepted)
        {
            Edge::Ptr edge = EdgeTypeManager::getInstance()->createEdge(
                dialog.getClassname().toStdString(), sourceVertex, targetVertex,
                dialog.getLabel().toStdString());
            getGraph()->addEdge(edge);
            // TODO: is there a nicer way of trigger redrawing the graph?
            getGraphWidget()->clearVisualization();
            getGraphWidget()->update();
        }
    }
}

} // end namespace gui
} // end namespace graph_analysis
