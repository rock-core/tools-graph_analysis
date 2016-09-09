#ifndef GRAPH_ANALYSIS_GUI_GVLAYOUT_HPP
#define GRAPH_ANALYSIS_GUI_GVLAYOUT_HPP

#include <graph_analysis/gui/GraphLayout.hpp>

namespace graph_analysis {
namespace io {
    class GVGraph;
} // end namespace io

namespace gui {
namespace layouts {

class GVLayout : public graph_analysis::gui::GraphLayout
{
public:
    GVLayout();
    virtual ~GVLayout();

    virtual std::string getClassName() const { return "graph_analysis::gui::layouts::GVLayout"; }

    void initialize(const graph_analysis::BaseGraph::Ptr& graph);

    GraphWidget::VertexItemCoordinateCache getCoordinates(const graph_analysis::BaseGraph::Ptr& graph) const;

    void update(const graph_analysis::BaseGraph::Ptr& graph, const std::string& layoutName = "");

    std::set<std::string> getSupportedLayouts() const;

protected:
    void applyLayout(const std::string& name);

    graph_analysis::io::GVGraph* mpLayoutGraph;
    std::string mDefaultLayoutName;
    std::string mGraphName;
};

} // end namespace layouts
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GVLAYOUT_HPP
