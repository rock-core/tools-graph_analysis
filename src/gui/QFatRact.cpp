#include "QFatRact.hpp"
#include <QPen>

QFatRact::QFatRact(QGraphicsItem* parent, QColor color)
    : QGraphicsRectItem(parent)
{
    setPen(QPen(QBrush(color), getPlannedPenWidth(), Qt::SolidLine,
                Qt::SquareCap, Qt::MiterJoin));
}
void QFatRact::paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
                     QWidget* widget)
{
    QGraphicsRectItem::paint(painter, option, widget);
}
int QFatRact::getPlannedPenWidth()
{
    return 1;
}
