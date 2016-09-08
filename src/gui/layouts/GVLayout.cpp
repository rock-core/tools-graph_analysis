#include "GVLayout.hpp"
#include <graph_analysis/io/GVGraph.hpp>
#include <sstream>

#include <base-logging/Logging.hpp>

namespace graph_analysis{
namespace gui {
namespace layouts {

GVLayout::GVLayout(const graph_analysis::BaseGraph::Ptr& graph,
            const std::string& layoutName,
            const std::string& graphName)
    : GraphLayout(graph)
    , mpLayoutGraph(NULL)
    , mLayoutName(layoutName)
    , mGraphName(graphName)
{
    update();
}

GVLayout::~GVLayout()
{
    delete mpLayoutGraph;
}

void GVLayout::initialize()
{
    std::string name = mGraphName;
    if(name.empty())
    {
        std::stringstream ss;
        ss << "graph_" << mpBaseGraph->getId();
        name = ss.str();
    }

    delete mpLayoutGraph;
    mpLayoutGraph = new graph_analysis::io::GVGraph(mpBaseGraph, name);
    mpLayoutGraph->initializeFromBaseGraph();
}

void GVLayout::update(const std::string& layoutName)
{
    std::string selectedLayout = mLayoutName;
    if(!layoutName.empty())
    {
        selectedLayout = layoutName;
    }
    initialize();
    LOG_WARN_S << "Applying layout: " << selectedLayout;
    mpLayoutGraph->applyLayout(selectedLayout);
}

GraphWidget::VertexItemCoordinateCache GVLayout::getCoordinates() const
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
    return mpLayoutGraph->getSupportedLayouts();
}

} // end namespace layouts
} // end namespace gui
} // end namespace graph_analysis
