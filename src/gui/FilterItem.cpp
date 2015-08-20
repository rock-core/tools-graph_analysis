#include "FilterItem.hpp"
#include "FilterManager.hpp"

#include <QStyle>
#include <QPainter>
#include <QStyleOption>
#include <QGraphicsScene>
#include <QGraphicsSceneDragDropEvent>
#include <graph_analysis/gui/layeritem/Label.hpp>

#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

const FilterItem::filter_size_t FilterItem::sHeight = 30;
const qreal FilterItem::sDisplacementWeight = 0.169;
const qreal FilterItem::sDisplacementThreshold = FilterItem::sDisplacementWeight * (qreal)FilterItem::sHeight;

FilterItem::FilterItem(FilterManager *manager, filter_index_t index, const std::string& filter)
    : mpFilterManager(manager)
    , mIndex(index)
    , mPen(Qt::black)
    , mPenDefault(Qt::black)
{
    setFlag(ItemIsMovable);
    setFlag(ItemSendsGeometryChanges);
    setCacheMode(DeviceCoordinateCache);
    setZValue(-1);
    setToolTip(QString("Custom Filter '") + QString(filter.c_str()) + QString("'"));
    //setFlag(QGraphicsTextItem::ItemIsSelectable, true);
    mLabel = new layeritem::Label(filter, this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setParentItem(this);
    //mLabel->setTextInteractionFlags(Qt::TextEditorInteraction);
    //mLabel->setFlag(QGraphicsItem::ItemIsSelectable, true);
    //mLabel->setZValue(-100.0);
}

void FilterItem::setLabel(QString label)
{
    mLabel->setPlainText(label);
}

QString FilterItem::getLabel(void)
{
    return mLabel->toPlainText();
}

void FilterItem::updatePos(void)
{
    setPos(0., (qreal)(FilterItem::sHeight * mIndex));
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
    setZValue(0.);
    QGraphicsItem::mousePressEvent(event);
}

void FilterItem::mouseMoveEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse move";
    // testing for new index displacement on neighbouring filters (TODO: add one for final index placement of current filter in mouse release)
    qreal y = pos().y();
    const unsigned int offset = (unsigned int)(y / (qreal)FilterItem::sHeight);
    if(offset != mIndex && FilterItem::sDisplacementThreshold > y - (qreal)(offset * FilterItem::sHeight))
    {
        mpFilterManager->swapFilters(mIndex, offset); // offset <-> mIndex
    }
    else if(offset + 1 != mIndex && FilterItem::sDisplacementThreshold > (qreal)((offset + 1) * FilterItem::sHeight) - y)
    {
        mpFilterManager->swapFilters(mIndex, offset + 1); // offset + 1 <-> mIndex
    }
    QGraphicsItem::mouseMoveEvent(event);
}

void FilterItem::mouseReleaseEvent(::QGraphicsSceneMouseEvent* event)
{
    LOG_DEBUG_S << "Mouse RESOURCE: release";
    setZValue(-1);
    updatePos();
    QGraphicsItem::mouseReleaseEvent(event);
}

void FilterItem::mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event)
{
    mLabel->setTextInteraction(true, true);
    QGraphicsItem::mouseDoubleClickEvent(event);
    setHandlesChildEvents(false);
}

void FilterItem::hoverEnterEvent(QGraphicsSceneHoverEvent *event)
{
    LOG_DEBUG_S << "Hover ENTER event for filter " << mLabel->toPlainText().toStdString();
    mPen = QPen(Qt::green);
    mpFilterManager->setItemSelected(true);
    mpFilterManager->setSelectedItem(this);
    QGraphicsItem::hoverEnterEvent(event);
}

void FilterItem::hoverLeaveEvent(QGraphicsSceneHoverEvent *event)
{
    LOG_DEBUG_S << "Hover LEAVE event for filter " << mLabel->toPlainText().toStdString();
    mPen = mPenDefault;
    mpFilterManager->setItemSelected(false);
    QGraphicsItem::hoverLeaveEvent(event);
}

QVariant FilterItem::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if (change == ItemPositionChange && scene())
    {
        // value is the new position.
        QPointF newPos = value.toPointF();
        // adjusting the new position to fall inside the height boundaries and alligned on the x = 0 axis
        newPos.setX(0.);
        qreal y = newPos.y();
        if(0. > y)
        {
            newPos.setY(0.);
        }
        else
        {
            qreal maxY = (qreal)(mpFilterManager->filterCount() - 1) * FilterItem::sHeight;
            if(maxY < y)
            {
                newPos.setY(maxY);
            }
        }
        return newPos;
    }
    return QGraphicsItem::itemChange(change, value);
}

//void FilterItem::keyPressEvent(QKeyEvent* event)
//{
//    LOG_DEBUG_S << "Key RESOURCE: press";
//}

} // end namespace gui
} // end namespace graph_analysis