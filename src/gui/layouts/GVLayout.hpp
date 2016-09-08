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
    GVLayout(const graph_analysis::BaseGraph::Ptr& graph,
            const std::string& layoutName = "dot",
            const std::string& graphName = "");

    virtual ~GVLayout();

    void initialize();

    GraphWidget::VertexItemCoordinateCache getCoordinates() const;

    void update(const std::string& layoutName = "");

    void applyLayout(const std::string& name);

    std::set<std::string> getSupportedLayouts() const;

protected:
    graph_analysis::io::GVGraph* mpLayoutGraph;
    std::string mLayoutName;
    std::string mGraphName;

};

} // end namespace layouts
} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GVLAYOUT_HPP
