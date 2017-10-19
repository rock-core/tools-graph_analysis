#include "DirectedSubGraph.hpp"
#include "../BaseGraph.hpp"

namespace graph_analysis {
namespace snap {

DirectedSubGraph::DirectedSubGraph(const BaseGraph::Ptr& graph)
    : SubGraph(graph)
    , mDirectedGraph(graph->copy())
{
}

void DirectedSubGraph::enable(const Vertex::Ptr& vertex)
{
    if(getBaseGraph()->contains(vertex))
    {
        mDirectedGraph->addVertex(vertex);
    } else {
        throw std::invalid_argument("graph_analysis::snap::DirectedSubGraph::enable could not enable vertex -- does not exist in base graph");
    }

}

void DirectedSubGraph::disable(const Vertex::Ptr& vertex)
{
    try {
        mDirectedGraph->removeVertex(vertex);
    } catch(...)
    {}
}

void DirectedSubGraph::enable(const Edge::Ptr& edge)
{
    if(getBaseGraph()->contains(edge))
    {
        mDirectedGraph->addEdge(edge);
    } else {
        throw std::invalid_argument("graph_analysis::snap::DirectedSubGraph::enable could not enable edge -- does not exist in base graph");
    }

}

void DirectedSubGraph::disable(const Edge::Ptr& edge)
{
    try {
        mDirectedGraph->removeEdge(edge);
    } catch(...)
    {}
}

bool DirectedSubGraph::enabled(const Vertex::Ptr& vertex) const
{
    return mDirectedGraph->contains(vertex);
}

bool DirectedSubGraph::enabled(const Edge::Ptr& edge) const
{
    return mDirectedGraph->contains(edge);
}

} // end namespace snap
} // end namespace graph_analysis
