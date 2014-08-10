#include "Resource.hpp"

#include <QPainter>
#include <QStyle>
#include <QStyleOption>

#include <base/Logging.hpp>

#include "Label.hpp"

namespace omviz {
namespace graphitem {

Resource::Resource(GraphWidget* graphWidget, graph_analysis::Vertex::Ptr vertex)
    : NodeItem(graphWidget, vertex)
    , mBrushDefault(Qt::blue)
    , mBrush(Qt::blue)
{
    //setFlag(QGraphicsTextItem::ItemIsSelectable, true);
    mLabel = new Label(vertex->toString(), this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setParentItem(this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //mLabel->setZValue(-100.0);
}

QRectF Resource::boundingRect() const
{
    //qreal adjust = 0;
    //QRectF boundingRect( -10 - adjust, -10 - adjust, 23 + adjust, 23 + adjust);
    //return childrenBoundingRect() | boundingRect;

    return childrenBoundingRect();
}

QPainterPath Resource::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void Resource::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* )
{
    painter->setPen(Qt::NoPen);
    painter->setBrush(mBrush);
    //painter->drawEllipse(-7, -7, 20, 20);
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


void Resource::mousePressEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: press";
    //mLabel->mouseEvent(event);
}

void Resource::mouseReleaseEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: release";
}
void Resource::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
}

void Resource::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug("Hover ENTER event for %s", mpVertex->toString().c_str());
    mBrush = QBrush(Qt::green);

    QGraphicsItem::hoverEnterEvent(event);
}

void Resource::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    qDebug("Hover LEAVE event for %s", mpVertex->toString().c_str());
    mBrush = QBrush(mBrushDefault);
    QGraphicsItem::hoverLeaveEvent(event);
}

//void Resource::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

}
}
