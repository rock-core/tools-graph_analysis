#include "GraphvizWriter.hpp"
#include <base/Logging.hpp>
#include "GVGraph.hpp"

namespace graph_analysis {
namespace io {

GraphvizWriter::GraphvizWriter(const std::string &layout) : mLayout(layout)
{}

GraphvizWriter::~GraphvizWriter()
{}

void GraphvizWriter::write(const std::string& filename, const BaseGraph& graph) const
{
    GVGraph gvGraph(graph.copy(), "GraphvizGraph");

    VertexIterator::Ptr nodeIt = graph.getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();
        gvGraph.addNode(vertex);
    }

    EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        gvGraph.addEdge(edge);
    }

    LOG_INFO("GraphvizWriter: Applying default layout such that GVGraph context is not empty");
    gvGraph.applyLayout();
    LOG_INFO("GraphvizWriter: rendering GVGraph to file \"%s\" by layout \"%s\"", filename.c_str(), mLayout.c_str());
    gvGraph.renderToFile(filename, mLayout);
    LOG_INFO("GraphvizWriter: done rendering GVGraph to file \"%s\"", filename.c_str());
}

} // end namespace io
} // end namespace graph_analysis
