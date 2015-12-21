#include "DirectedSubGraph.hpp"
#include <graph_analysis/BaseGraph.hpp>

#include <graph_analysis/SharedPtr.hpp>

namespace graph_analysis {
namespace snap {

DirectedSubGraph::DirectedSubGraph(const BaseGraph::Ptr& graph)
    : SubGraph(graph)
    , mDirectedGraph(graph->copy())
{
}

void DirectedSubGraph::enable(Vertex::Ptr vertex)
{
    if(getBaseGraph()->contains(vertex))
    {
        mDirectedGraph->addVertex(vertex);
    } else {
        throw std::invalid_argument("graph_analysis::snap::DirectedSubGraph::enable could not enable vertex -- does not exist in base graph");
    }

}

void DirectedSubGraph::disable(Vertex::Ptr vertex)
{
    try {
        mDirectedGraph->removeVertex(vertex);
    } catch(...)
    {}
}

void DirectedSubGraph::enable(Edge::Ptr edge)
{
    if(getBaseGraph()->contains(edge))
    {
        mDirectedGraph->addEdge(edge);
    } else {
        throw std::invalid_argument("graph_analysis::snap::DirectedSubGraph::enable could not enable edge -- does not exist in base graph");
    }

}

void DirectedSubGraph::disable(Edge::Ptr edge)
{
    try {
        mDirectedGraph->removeEdge(edge);
    } catch(...)
    {}
}

bool DirectedSubGraph::enabled(Vertex::Ptr vertex) const
{
    return mDirectedGraph->contains(vertex);
}

bool DirectedSubGraph::enabled(Edge::Ptr edge) const
{
    return mDirectedGraph->contains(edge);
}

} // end namespace snap
} // end namespace graph_analysis
