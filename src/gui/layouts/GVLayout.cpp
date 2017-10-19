#include "GVLayout.hpp"
#include "../../io/GVGraph.hpp"
#include <sstream>

#include <base-logging/Logging.hpp>

namespace graph_analysis{
namespace gui {
namespace layouts {

GVLayout::GVLayout()
    : GraphLayout()
    , mpLayoutGraph(new graph_analysis::io::GVGraph(graph_analysis::BaseGraph::getInstance(), "default-basegraph"))
    , mDefaultLayoutName("dot")
{
}

GVLayout::~GVLayout()
{
    delete mpLayoutGraph;
}

void GVLayout::initialize(const graph_analysis::BaseGraph::Ptr& graph)
{
    std::stringstream ss;
    ss << "graph_" << graph->getId();

    delete mpLayoutGraph;
    mpLayoutGraph = new graph_analysis::io::GVGraph(graph, ss.str());
    mpLayoutGraph->initializeFromBaseGraph();
}

void GVLayout::update(const graph_analysis::BaseGraph::Ptr& graph,
        const std::string& layoutName,
        GraphWidget::VertexItemMap& vertexItemMap,
        QGraphicsScene* scene)
{
    std::string selectedLayout = mDefaultLayoutName;
    if(!layoutName.empty())
    {
        selectedLayout = layoutName;
    }

    // This the cleanup of gv is not very robust (bugs in GV)
    // , we recreate structure
    initialize(graph);

    assert(mpLayoutGraph);
    mpLayoutGraph->applyLayout(selectedLayout);
}

GraphWidget::VertexItemCoordinateCache GVLayout::getCoordinates(const graph_analysis::BaseGraph::Ptr& graph) const
{
    using namespace graph_analysis::io;

    GraphWidget::VertexItemCoordinateCache coordinates;
    std::vector<GVNode> nodes = mpLayoutGraph->nodes();

    std::vector<GVNode>::const_iterator cit = nodes.begin();
    for(; cit != nodes.end(); ++cit)
    {
        const GVNode& node = *cit;
        QPointF position(node.x(), node.y());
        coordinates[node.getVertex()] = position;
    }
    return coordinates;
}

std::set<std::string> GVLayout::getSupportedLayouts() const
{
    assert(mpLayoutGraph);
    return mpLayoutGraph->getSupportedLayouts();
}

} // end namespace layouts
} // end namespace gui
} // end namespace graph_analysis
