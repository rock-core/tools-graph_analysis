#include <base-logging/Logging.hpp>
#include "GraphvizWriter.hpp"
#include "GVGraph.hpp"

namespace graph_analysis {
namespace io {

GraphvizWriter::GraphvizWriter(const std::string& layout)
    : mLayout(layout)
    , mpStyle()
{}

GraphvizWriter::~GraphvizWriter()
{}

void GraphvizWriter::write(const std::string& filename, const BaseGraph& graph) const
{
    BaseGraph graph_copy = graph;
    write(filename, BaseGraph::Ptr(&graph_copy)); // reusing code
}

void GraphvizWriter::write(const std::string& filename, const BaseGraph::Ptr& graph) const
{
    // initializing graphViz instance
    GVGraph gvGraph(graph, "GraphvizGraph");
    // populating it with the nodes
    VertexIterator::Ptr nodeIt = graph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex = nodeIt->current();
        gvGraph.addNode(vertex);
        if(mpStyle)
        {
            mpStyle->apply(vertex, &gvGraph, graph);
        }
    }

    // populating it with the edges
    EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        gvGraph.addEdge(edge);
        if(mpStyle)
        {
            mpStyle->apply(edge, &gvGraph, graph);
        }
    }
    // layouting and rendering
    LOG_INFO("GraphvizWriter: Applying default layout such that GVGraph context is not empty");
    gvGraph.applyLayout();
    LOG_INFO("GraphvizWriter: rendering GVGraph to file \"%s\" by layout \"%s\"", filename.c_str(), mLayout.c_str());
    gvGraph.renderToFile(filename, mLayout); // at the rendering stage, the graph is relayouted internally
    LOG_INFO("GraphvizWriter: done rendering GVGraph to file \"%s\"", filename.c_str());
}

} // end namespace io
} // end namespace graph_analysis
