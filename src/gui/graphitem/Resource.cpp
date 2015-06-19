#include "Resource.hpp"

#include <QPainter>
#include <QGraphicsSceneDragDropEvent>
#include <QStyle>
#include <QStyleOption>

#include <base/Logging.hpp>
#include "Label.hpp"
#define ADJUST 23.69

namespace graph_analysis {
namespace gui {
namespace graphitem {

Resource::Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
    : NodeItem(graphWidget, vertex)
    , mPen(Qt::blue)
    , mPenDefault(Qt::blue)
{
    //setFlag(QGraphicsTextItem::ItemIsSelectable, true);
    mLabel = new Label(vertex->toString(), this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setParentItem(this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //mLabel->setZValue(-100.0);
//    setHandlesChildEvents(false);
    setFlag(ItemIsMovable);
}

void Resource::changeLabel(const std::string& label)
{
    delete mLabel;
    mpVertex->setLabel(label);
    mLabel = new Label(mpVertex->toString(), this);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

void Resource::updateLabel()
{
    delete mLabel;
    mLabel = new Label(mpVertex->toString(), this);
    this->itemChange(QGraphicsItem::ItemPositionHasChanged, QVariant());
}

QRectF Resource::boundingRect() const
{
    //QRectF boundingRect( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
    //return childrenBoundingRect() | boundingRect;

    QRectF childrenRect = childrenBoundingRect();
//    if("graph_analysis::ClusterVertex" == mpVertex->getClassName())
//    {
//        qreal adjust = 13.;
//        return childrenRect.adjusted( - adjust, - adjust, adjust, adjust + (qreal) ((1 + mLabels.size()) >> 1) * 2. * adjust);
//    }
    return childrenRect;
}

QPainterPath Resource::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Resource::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* )
{
    // Draws fully filled item
    //painter->setPen(Qt::NoPen);
    //painter->setBrush(mPen.brush());

    //painter->drawEllipse(-7, -7, 20, 20);

    painter->setPen(QPen(Qt::black, 0));
    int port_count = mLabels.size();
    for(int i = 0; i < port_count; ++i)
    {
        painter->drawPolygon(portBoundingRect(i));
    }
    // Drawing of border: back to transparent background
    painter->setPen(mPen);
    painter->drawRect(boundingRect()); //-7,-7,20,20);

//    QRadialGradient gradient(-3, -3, 10);
//    if (option->state & QStyle::State_Sunken)
//    {
//        gradient.setCenter(3, 3);
//        gradient.setFocalPoint(3, 3);
//        gradient.setColorAt(1, QColor(Qt::yellow).light(120));
//        gradient.setColorAt(0, QColor(Qt::darkYellow).light(120));
//    } else {
//        gradient.setColorAt(0, Qt::yellow);
//        gradient.setColorAt(1, Qt::darkYellow);
//    }
//    painter->setBrush(gradient);
//
//    painter->setPen(QPen(Qt::black, 0));
//    painter->drawEllipse(-10, -10, 20, 20);
}

int Resource::addPort(Vertex::Ptr node)
{
    int size = mLabels.size();
    Label *label = new Label(node->getLabel(), this, size);
    mLabels.push_back(label);
//    addToGroup(label);
    label->setPos(mLabel->pos() + QPointF(0., qreal(2 + (size << 1)) * qreal(ADJUST)));
    return size; // returning this port's offset in the vector of ports
}


QPolygonF Resource::portBoundingRect(int portID)
{
    QRectF result = boundingRect();
    result.adjust(0, result.height() - qreal(2 + 2 * portID) * ADJUST, 0, - qreal(2 * portID) * ADJUST);
//    result.adjust(0,  qreal(1 + 2 * portID) * ADJUST, 0, qreal(3 + 2 * portID) * ADJUST - result.height());
    return QPolygonF(result);
}

void Resource::mousePressEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: press";
    bool dragDrop = mpGraphWidget->getDragDrop();
    if(dragDrop)
    {
        mpGraphWidget->startNewEdgeHere();
    }
    QGraphicsItem::mousePressEvent(event);
}

void Resource::mouseReleaseEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: release";
    QGraphicsItem::mouseReleaseEvent(event);
}

void Resource::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void Resource::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    bool dragDrop = mpGraphWidget->getDragDrop();
    if(dragDrop)
    {
        setFlag(ItemIsMovable, false);
        if(mpGraphWidget->getDragInitiated() && mpGraphWidget->getDragSource() != mpVertex)
        {
            mpGraphWidget->setSelectedVertex(mpVertex);
            mpGraphWidget->setVertexSelected(true);
            mpGraphWidget->endNewEdgeHere();
        }
    }
    else if("graph_analysis::ClusterVertex" == mpVertex->getClassName())
    {
        setFlag(ItemIsMovable);
    }
    qDebug("Hover ENTER event for %s", mpVertex->toString().c_str());
    mPen = QPen(Qt::green);

    mpGraphWidget->setSelectedVertex(mpVertex);
    mpGraphWidget->setVertexSelected(true);
//    qDebug("Hover event -> set mVertexSelected flag to %d", mpGraphWidget->getVertexSelected());

    QGraphicsItem::hoverEnterEvent(event);
}

void Resource::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug("Hover LEAVE event for %s", mpVertex->toString().c_str());
    mPen = mPenDefault;
    mpGraphWidget->setVertexSelected(false);
//    qDebug("Hover event -> set mVertexSelected flag to %d", mpGraphWidget->getVertexSelected());
    QGraphicsItem::hoverLeaveEvent(event);
}

//void Resource::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace graphitem
} // end namespace gui
} // end namespace graph_analysis
