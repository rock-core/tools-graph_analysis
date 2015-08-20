#ifndef FILTERITEM_HPP
#define FILTERITEM_HPP

#include <QPen>
#include <QObject>
#include <string>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>

namespace graph_analysis {
namespace gui {

namespace layeritem {
    class Label;
} // end namespace layeritem

class FilterManager;

/**
 * \file FilterItem.hpp
 * \class FilterItem
 * \brief graphical custom regexp filter representation class; specific to FilterManager widget in the layers tab
 */
class FilterItem : public QGraphicsItemGroup
{
public:
    typedef unsigned int filter_size_t;
    typedef unsigned int filter_index_t;

    static const filter_size_t sHeight;
    static const qreal sDisplacementWeight;
    static const qreal sDisplacementThreshold;
    /// constructor
    FilterItem(FilterManager *manager, filter_index_t index, const std::string& filter = std::string());
    /// destructor
    ~FilterItem() {};

    /// retrieves the bounding rectangular box around the area occupied by the node in the scene
    QRectF boundingRect() const;
    /// retrieves the path of the bounding rectangular box around the area occupied by the node in the scene
    QPainterPath shape() const;
    /// qt node painting method - here the node components get placed in the scene
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget*);

    void setIndex(filter_index_t index) { mIndex = index; }
    void updatePos(void);
    QString getLabel(void);
    void setLabel(QString label);

protected:
    /// qt mouse double-click callback
    void mouseDoubleClickEvent(::QGraphicsSceneMouseEvent* event);
    /// qt mouse move callback
    void mouseMoveEvent(::QGraphicsSceneMouseEvent* event);
    /// qt mouse press callback
    void mousePressEvent(::QGraphicsSceneMouseEvent* event);
    /// qt mouse release callback
    void mouseReleaseEvent(::QGraphicsSceneMouseEvent* event);

    /// qt hovering ENTER callback
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    /// qt hovering LEAVE callback
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    /// qt item change callback
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

private:
    /// parent widget
    FilterManager* mpFilterManager;
    /// index in the vector of filters (indirectly indicates position)
    filter_index_t mIndex;
    /// main node text label
    layeritem::Label* mLabel;
    /// qt drawing pen
    QPen mPen;
    /// default qt drawing pen
    QPen mPenDefault;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // FILTERITEM_HPP