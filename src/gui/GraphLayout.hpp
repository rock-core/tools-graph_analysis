#ifndef GRAPH_ANALYSIS_GUI_GRAPHLAYOUT_HPP
#define GRAPH_ANALYSIS_GUI_GRAPHLAYOUT_HPP

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {

class GraphLayout
{
public:
    /**
     * Create Graph Layout using the base graph
     */
    GraphLayout(const graph_analysis::BaseGraph::Ptr& graph);

    /**
     * Default destructor
     */
    virtual ~GraphLayout();

    /**
     * Get the coordinates of the layout
     */
    virtual GraphWidget::VertexItemCoordinateCache getCoordinates() const { throw std::runtime_error("graph_analysis::gui::GraphLayout::getCoordinates not implemented"); }

    /**
     * Update the current layout, e.g. after changes to the underlying graph
     */
    virtual void update(const std::string& layoutName = "") { throw std::runtime_error("graph_analysis::gui::GraphLayout::update not implemented"); }

    graph_analysis::BaseGraph::Ptr getBaseGraph() const { return mpBaseGraph; }

    virtual std::set<std::string> getSupportedLayouts() const { throw std::runtime_error("graph_analysis::gui::GraphLayout::getSupportedLayouts: not implemented"); }

protected:
    graph_analysis::BaseGraph::Ptr mpBaseGraph;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPHLAYOUT_HPP
