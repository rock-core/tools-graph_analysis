#ifndef GRAPH_ANALYSIS_GUI_GRAPH_LAYOUT_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_GRAPH_LAYOUT_MANAGER_HPP

#include <base-logging/Singleton.hpp>
#include <graph_analysis/gui/GraphLayout.hpp>

namespace graph_analysis {
namespace gui {

class GraphLayoutManager : public base::Singleton<GraphLayoutManager>
{
    friend class base::Singleton<GraphLayoutManager>;

protected:
    GraphLayoutManager();

public:
    ~GraphLayoutManager();

    void registerLayout(const GraphLayout::Ptr& ptr);

    GraphWidget::VertexItemCoordinateCache getCoordinates(const graph_analysis::BaseGraph::Ptr& graph,
            const std::string& layoutName,
            GraphWidget::VertexItemMap& vertexItemMap,
            QGraphicsScene* scene = NULL
            ) const;

    std::set<std::string> getSupportedLayouts() const;

    GraphLayout::Ptr getGraphLayout(const std::string& layoutName) const;

private:
    std::map<std::string, GraphLayout::Ptr> mLayouts;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_GRAPH_LAYOUT_MANAGER_HPP
