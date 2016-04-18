#include "VertexItemBase.hpp"

#include "GraphWidget.hpp"

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QDebug>
#include <QDrag>

#include <base/Logging.hpp>
#include <graph_analysis/gui/items/Feature.hpp>
#include <graph_analysis/gui/BaseGraphView/BaseGraphView.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/EdgeMimeData.hpp>
#include <graph_analysis/gui/BaseGraphView/AddEdgeDialog.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

namespace graph_analysis
{
namespace gui
{

VertexItemBase::VertexItemBase(GraphWidget* graphWidget,
                               graph_analysis::Vertex::Ptr vertex,
                               QGraphicsItem* parent)
    : QGraphicsItem(parent)
    , mpVertex(vertex)
    , mpGraphWidget(graphWidget)
{
    // this enabled "itemChange()" notifications. when this item moves, it has
    // to tell its edges to follow it, so they stay visually connected. this is
    // done by calling "adjust()" for the respective edge
    setFlag(ItemSendsScenePositionChanges);
    // this cache-mode is for items that can move. not sure if we can move --
    // vertices can move?
    setCacheMode(DeviceCoordinateCache);
    // with this we tell the graphwidget that "this" is responsible to
    // represent "mpVertex"
    //
    // shouldn't this be moved elsewhere?
    mpGraphWidget->registerVertexItem(mpVertex, this);
    // drag'n drop is used to create edges between vertices
    setAcceptHoverEvents(true);
}

VertexItemBase::~VertexItemBase()
{
    mpGraphWidget->deregisterVertexItem(mpVertex, this);
}

BaseGraph::Ptr VertexItemBase::getGraph() const
{
    return mpGraphWidget->graph();
}

void VertexItemBase::hoverEnterEvent(QGraphicsSceneHoverEvent* event)
{
    // set the underlaying vertex as focused element in the statusbar
    mpGraphWidget->setFocusedElement(mpVertex);
    QGraphicsItem::hoverEnterEvent(event);
}

void VertexItemBase::hoverLeaveEvent(QGraphicsSceneHoverEvent* event)
{
    // set focused element in the statusbar
    mpGraphWidget->clearFocus();
    QGraphicsItem::hoverLeaveEvent(event);
}

QVariant VertexItemBase::itemChange(GraphicsItemChange change,
                                    const QVariant& value)
{
    switch(change)
    {
    case ItemScenePositionHasChanged:
    {
        // notify the graph widget about our new position. there, relevant
        // caching and updating of connected items is performed.
        getGraphWidget()->vertexPositionHasChanged(this);
        break;
    }
    default:
    {
        break;
    }
    };
    return QGraphicsItem::itemChange(change, value);
}

/**
 *
 * kiss: very simple base implementation of a rectangle
 *
 */
VertexItemSimple::VertexItemSimple(GraphWidget* graphWidget,
                                   graph_analysis::Vertex::Ptr vertex,
                                   QGraphicsItem* parent)
    : VertexItemBase(graphWidget, vertex, parent)
{

    // at the lowest (so in the background) the rectangle
    mpRect = new QGraphicsRectItem(this);
    mpRect->setPen(QPen(Qt::blue));

    mpLabel = new QGraphicsTextItem(QString(vertex->getLabel().c_str()), this);
    QFont font = mpLabel->font();
    font.setBold(true);
    mpLabel->setFont(font);

    mpClassName =
        new QGraphicsTextItem(QString(vertex->getClassName().c_str()), this);
    mpClassName->setPos(mpLabel->pos() +
                        QPoint(0, mpLabel->boundingRect().height()));
    mpClassName->setDefaultTextColor(Qt::gray);
    // we wanna show the current qt-coordinate on the canvas
    mpCoordinate = new QGraphicsTextItem(getScenePosAsString(), this);
    mpCoordinate->setDefaultTextColor(Qt::darkGreen);

    // now that all the children are there, we use their bounding-rect to
    // enlarge the background-rect. note that we never modify the boundingRect
    // afterwards.
    mpRect->setRect(childrenBoundingRect());

    // change the position of the "coordinate" label to be pinned in the
    // top-right corner of the blue rect
    mpCoordinate->setPos(mpRect->rect().topRight()-
                         QPointF(mpCoordinate->boundingRect().width(), 0));

    // for this "Simple" type we want to have it movable. this graphical
    // "object" will not be contained inside other items, so thats ok
    setFlag(ItemIsMovable);

    // for thi specific "Simple" implmentation, we accept drops -- and allow
    // drops -- to create new edges. see later.
    setAcceptDrops(true);
}

VertexItemSimple::~VertexItemSimple()
{
    delete mpLabel;
    delete mpClassName;
    delete mpCoordinate;
    delete mpRect;
}

void VertexItemSimple::paint(QPainter* painter,
                             const QStyleOptionGraphicsItem* option,
                             QWidget* widget)
{
}

QRectF VertexItemSimple::boundingRect() const
{
    return childrenBoundingRect();
}

// drag'n drop!

// this function is called as long as it is not "accepted" (by calling the
// relevant function.
void VertexItemSimple::dragEnterEvent(QGraphicsSceneDragDropEvent* event)
{
    // by default, this is not accepted.
    event->setAccepted(false);
    // checking that the vertex stored in the mimetype is different from ours
    // -- this prevents self-drag'n drops
    const EdgeMimeData* pMimeData =
        dynamic_cast<const EdgeMimeData*>(event->mimeData());
    if(pMimeData)
    {
        if(!pMimeData->sourceVertexIsSameAs(getVertex()))
        {
            // after this, the mouse icon will change and the drag-object will
            // be active
            event->setAccepted(true);
            mpRect->setBrush(Qt::green);
        }
        else
        {
            // TODO: would be nice to give visual feedback on the "non
            // acceptance" -- change coloring to red?
            return;
        }
    }
    else
    {
        LOG_ERROR_S << "unexpected pointer type for mimedata?";
    }
}

void VertexItemSimple::dragLeaveEvent(QGraphicsSceneDragDropEvent* event)
{
    mpRect->setBrush(Qt::NoBrush);
}

void VertexItemSimple::dropEvent(QGraphicsSceneDragDropEvent* event)
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

void VertexItemSimple::mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event)
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
        }
    }
}

QString VertexItemSimple::getScenePosAsString() const {
    return QString("(%1, %2)").arg(scenePos().x()).arg(scenePos().y());
}

QVariant VertexItemSimple::itemChange(GraphicsItemChange change,
                                    const QVariant& value)
{
    switch(change)
    {
    case ItemScenePositionHasChanged:
    {
        // notify the graph widget about our new position. there, relevant
        // caching and updating of connected items is performed.
        mpCoordinate->setPlainText(getScenePosAsString());
        // and also move the whole label a bit to stay aligned with the rect
        mpCoordinate->setPos(mpRect->rect().topRight()-
                             QPointF(mpCoordinate->boundingRect().width(), 0));
        break;
    }
    default:
    {
        break;
    }
    };
    return VertexItemBase::itemChange(change, value);
}

} // end namespace gui
} // end namespace graph_analysis
