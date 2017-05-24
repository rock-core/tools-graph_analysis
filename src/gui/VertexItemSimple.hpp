#ifndef GRAPH_ANALYSIS_GUI_VERTEXITEMSIMPLE_HPP
#define GRAPH_ANALYSIS_GUI_VERTEXITEMSIMPLE_HPP

#include <graph_analysis/gui/VertexItemBase.hpp>

namespace graph_analysis {
namespace gui {

/**
 * Simplest possible implementation of a VertexItemBase
 * just a box with two strings -- type and
 * label
 */
class VertexItemSimple : public VertexItemBase
{
    friend class VertexItemTypeManager;

protected:
    VertexItemSimple();

public:
    VertexItemSimple(GraphWidget* graphWidget,
                     graph_analysis::Vertex::Ptr vertex, QGraphicsItem* parent);
    virtual ~VertexItemSimple();

    virtual int type() const
    {
        return VertexItemSimpleType;
    };

    /**
     * uses this (read only) notification to update the label showing current
     * canvas coordinate of the whole item. nice for debugging.
     */
    virtual QVariant itemChange(GraphicsItemChange change, const QVariant& value);

protected:
    /**
     * all items by default accept drag-n-drop events. override this function
     * in later classes to prevent certain drops?
     */
    void dragEnterEvent(QGraphicsSceneDragDropEvent* event);
    void dragLeaveEvent(QGraphicsSceneDragDropEvent* event);
    void dropEvent(QGraphicsSceneDragDropEvent* event);
    void mouseDoubleClickEvent(QGraphicsSceneMouseEvent* event);


    VertexItemBase* createNewItem(GraphWidget* graphWidget,
                const graph_analysis::Vertex::Ptr& vertex,
                QGraphicsItem* parent) const;

private:
    QGraphicsTextItem* mpLabel;
    QGraphicsTextItem* mpClassName;
    QGraphicsTextItem* mpCoordinate;
    QGraphicsRectItem* mpRect;

    /** convert the current scenePos of the item to smth like "(23, -14)" */
    QString getScenePosAsString() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_VERTEXITEMSIMPLE_HPP
