#ifndef GRAPH_ANALYSIS_GUI_GRAPHLAYOUT_HPP
#define GRAPH_ANALYSIS_GUI_GRAPHLAYOUT_HPP

#include "../BaseGraph.hpp"
#include "GraphWidget.hpp"

namespace graph_analysis {
namespace gui {

class GraphLayout
{
public:
    typedef shared_ptr<GraphLayout> Ptr;

    /**
     * Create Graph Layout
     */
    GraphLayout();

    /**
     * Default destructor
     */
    virtual ~GraphLayout();

    virtual std::string getClassName() const = 0;

    /**
     * Get the coordinates of the layout
     */
    virtual GraphWidget::VertexItemCoordinateCache getCoordinates(const graph_analysis::BaseGraph::Ptr& graph) const = 0;

    /**
     * Update the current layout, e.g. after changes to the underlying graph
     */
    virtual void update(const BaseGraph::Ptr& graph,
            const std::string& layoutName,
            GraphWidget::VertexItemMap& VertexItemMap,
            QGraphicsScene* scene) = 0;

    virtual std::set<std::string> getSupportedLayouts() const = 0;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHLAYOUT_HPP
