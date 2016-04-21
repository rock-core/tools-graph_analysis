#ifndef QFATRACT_H
#define QFATRACT_H

#include <QGraphicsRectItem>
#include <QColor>

class QFatRact : public QGraphicsRectItem
{
public:
    QFatRact(QGraphicsItem* parent, QColor color);
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option,
               QWidget* widget);
    static int getPlannedPenWidth();
};

#endif /*QFATRACT_H*/
