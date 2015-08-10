#include "FilterItem.hpp"

#include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsSceneDragDropEvent>
#include <graph_analysis/gui/layeritem/Label.hpp>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

FilterItem::FilterItem(const std::string& filter)
    : mPen(Qt::black)
    , mPenDefault(Qt::black)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setToolTip(QString(filter.c_str()));
    //setFlag(QGraphicsTextItem::ItemIsSelectable, true);
    mLabel = new layeritem::Label(filter, this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setParentItem(this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //mLabel->setZValue(-100.0);
}

QRectF FilterItem::boundingRect() const
{
    QRectF childrenRect = childrenBoundingRect();
    return childrenRect;
}

QPainterPath FilterItem::shape() const
{
    QPainterPath path;
    path.addRect(boundingRect());
    return path;
}

void FilterItem::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* )
{
    // Drawing of border, with transparent background
    painter->setPen(mPen);

    // Draws fully filled item
    //painter->setPen(Qt::NoPen);
    //painter->setBrush(mPen.brush());

    //painter->drawEllipse(-7, -7, 20, 20);
    painter->drawRect(boundingRect());

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


void FilterItem::mousePressEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: press";
    QGraphicsItem::mousePressEvent(event);
}

void FilterItem::mouseReleaseEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: release";
    QGraphicsItem::mouseReleaseEvent(event);
}

void FilterItem::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    QGraphicsItem::mouseDoubleClickEvent(event);
}

void FilterItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    LOG_DEBUG_S << "Hover ENTER event for filter " << mLabel->toPlainText().toStdString();
    mPen = QPen(Qt::green);
    QGraphicsItem::hoverEnterEvent(event);
}

void FilterItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    LOG_DEBUG_S << "Hover LEAVE event for filter " << mLabel->toPlainText().toStdString();
    mPen = mPenDefault;
    QGraphicsItem::hoverLeaveEvent(event);
}

//void FilterItem::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace gui
} // end namespace graph_analysis