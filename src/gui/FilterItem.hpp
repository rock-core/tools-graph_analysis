#ifndef FILTERITEM_HPP
#define FILTERITEM_HPP

#include <QPen>
#include <QObject>
#include <string>
#include <QGraphicsItem>
#include <QGraphicsItemGroup>

namespace graph_analysis {
namespace gui {

namespace items {
    class Label;
} // end namespace items

class FilterManager;

/**
 * \file FilterItem.hpp
 * \class FilterItem
 * \brief graphical custom regexp filter representation class; specific to FilterManager widget in the layers tab
 * \details holds a graphical regex text item in the scene of all vertically disposed regex filters;
 *      it is subject to snap-to-grid and vertical-movement-only behaviours
 */
class FilterItem : public QGraphicsItemGroup
{
    friend class FilterManager;

public:
    /// datatype for denoting the height (in points) of a graphical filter item
    typedef unsigned int filter_size_t;
    /// datatype for the filters index counter
    typedef unsigned int filter_index_t;

    /// constant height (in points) of a graphical filter item
    static const filter_size_t sHeight;
    /// constant displacement procentage (degree up to which two filter items need to overlap in the scene such that one of them gets pushed away)
    static const qreal sDisplacementWeight;
    /// constant displacement threshold (relative distance up to which overlapping items are away from each other before one of them gets pushed away)
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

    /// setter method for the filter index
    void setIndex(filter_index_t index) { mIndex = index; }
    /// getter method for the filter index
    filter_index_t getIndex(void) { return mIndex; }

    /// refreshes position in the scene of this filter item based on its filter index
    void updatePos(void);

    /// getter method for retrieving the text regex of this filter item
    QString getLabel(void) const;
    /// setter method for updating the text regex of this filter item
    void setLabel(QString label);
    /// synchronizes the toolTip-s of this filter item and of its corresponding checkbox in the filters box with the current text regex of this filter item
    void syncToolTips(void);
    /// check if filter is activated
    bool isActivated() const { return mIsActivated; }

    QString getRegexp() const { return getLabel(); }

protected:
    /// qt mouse double-click callback
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);
    /// qt mouse move callback
    void mouseMoveEvent(QGraphicsSceneMouseEvent* event);
    /// qt mouse press callback
    void mousePressEvent(QGraphicsSceneMouseEvent* event);
    /// qt mouse release callback
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* event);

    /// qt hovering ENTER callback
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event);
    /// qt hovering LEAVE callback
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event);
    /// qt item change callback
    QVariant itemChange(GraphicsItemChange change, const QVariant &value);

    void setActivation(bool activation) { mIsActivated = activation; }

private:
    /// parent managing widget
    FilterManager* mpFilterManager;
    /// index in the vector of filters (indirectly dictates position in the scene)
    filter_index_t mIndex;
    /// graphical regex text label
    items::Label* mLabel;
    /// qt drawing pen
    QPen mPen;
    /// default qt drawing pen
    QPen mPenDefault;
    /// check if filter is activated
    bool mIsActivated;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // FILTERITEM_HPP
