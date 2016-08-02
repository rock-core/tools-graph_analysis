#ifndef QFATRACT_H
#define QFATRACT_H

#include <QGraphicsRectItem>
#include <QColor>

/**
 * @brief small convenience class: a rect which has a common pen
 *
 * this was intended to be a more complex derived class. upon implementation,
 * all this melted away to leave the bare: just the common pen... well.
 */
class QFatRact : public QGraphicsRectItem
{
public:
    /**
     * takes the given color and combine it with a sensible default-pen
     */
    QFatRact(QGraphicsItem* parent, QColor color);
    /**
     * @brief it shall not be allowd to change the width of this ract!
     *
     * you can change anything, but not the width!
     */
    void setPen(const QPen& suggestedPen);
    /**
     * this returns the common width of all fat-racts, so that it can be used in
     * different locations.
     */
    static int getPlannedPenWidth();
};

#endif /*QFATRACT_H*/
