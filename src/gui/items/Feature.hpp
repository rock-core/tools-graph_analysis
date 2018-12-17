#ifndef GRAPH_ANALYSIS_GUI_ITEMS_FEATURE_HPP
#define GRAPH_ANALYSIS_GUI_ITEMS_FEATURE_HPP

#include <iostream>
#include <QGraphicsSceneDragDropEvent>
#include <QGraphicsTextItem>
#include <QTextCursor>
#include <QMouseEvent>
#include <QMimeData>
#include <QDrag>

#include "../../Vertex.hpp"
#include "../../Edge.hpp"

#include "../GraphicsItemTypes.hpp"

namespace graph_analysis {
namespace gui {

class GraphWidget;

namespace items {

/**
 * \file Feature.hpp
 * \class Feature
 * \brief A feature directly maps to a vertex in a graph
 * \details specific to the diagram editor widget: editable text representation
 */
class Feature : public QGraphicsTextItem
{
private:
    // managing graph widget
    GraphWidget* mpGraphWidget;
    // Associated grapelement
    GraphElement::Ptr mpGraphElement;

public:
    /**
     * \brief constructor
     * \param element GraphElement that a feature should be attributed to
     * \param graphWidget The graph widget
     * \param parent The parent of this item
     */
    Feature(GraphElement::Ptr element, GraphWidget *graphWidget, QGraphicsItem* parent);

    /// getter method: retrieves the assigned ID
    GraphElement::Ptr getGraphElement() const { return mpGraphElement; }

    Edge::Ptr getEdge() const;
    Vertex::Ptr getVertex() const;

    int type() const { return FeatureItemType; }

    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget);

protected:

    void mousePressEvent(QGraphicsSceneMouseEvent *event);

};

} // end namespace items
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_ITEMS_LABEL_HPP
