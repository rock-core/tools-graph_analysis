#include "QFatRact.hpp"
#include <QPen>

QFatRact::QFatRact(QGraphicsItem* parent, QColor color)
    : QGraphicsRectItem(parent)
{
    setPen(QPen(QBrush(color), getPlannedPenWidth(), Qt::SolidLine,
                Qt::SquareCap, Qt::MiterJoin));
}
void QFatRact::setPen(const QPen& suggestedPen)
{
    QPen alteredPen = suggestedPen;
    alteredPen.setWidth(getPlannedPenWidth());
    QGraphicsRectItem::setPen(alteredPen);
}
int QFatRact::getPlannedPenWidth()
{
    return 1;
}
