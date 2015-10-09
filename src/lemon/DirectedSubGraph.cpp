#include "DirectedSubGraph.hpp"
#include "DirectedGraph.hpp"

namespace graph_analysis {
namespace lemon {

DirectedSubGraph::DirectedSubGraph(const DirectedGraph::Ptr& graph)
    : SubGraphImpl(graph, new vertex_filter_t(graph->raw()), new edge_filter_t(graph->raw()))
    , mpDirectedGraph(graph)
{
    // graph_t refers to the given subgraph
    // needs to be explicetly set
    setSubgraph( new graph_t(mpDirectedGraph->raw(), getVertexFilter(), getEdgeFilter()) );
}

void DirectedSubGraph::enable(Vertex::Ptr vertex)
{
    GraphElementId graphElementId = vertex->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Node n = mpDirectedGraph->raw().nodeFromId( graphElementId );

    raw().enable(n);
}

void DirectedSubGraph::disable(Vertex::Ptr vertex)
{
    GraphElementId graphElementId = vertex->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Node node = mpDirectedGraph->raw().nodeFromId( graphElementId );
    raw().disable(node);

    EdgeIterator::Ptr edgeIt = getBaseGraph()->getEdgeIterator(vertex);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        disable(edge);
    }
}

void DirectedSubGraph::enable(Edge::Ptr edge)
{
    GraphElementId graphElementId = edge->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Arc arc = mpDirectedGraph->raw().arcFromId( graphElementId );

    raw().enable(arc);
}

void DirectedSubGraph::disable(Edge::Ptr edge)
{
    GraphElementId graphElementId = edge->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Arc arc = mpDirectedGraph->raw().arcFromId( graphElementId );

    raw().disable(arc);
}

bool DirectedSubGraph::enabled(Vertex::Ptr vertex) const
{
    GraphElementId graphElementId = vertex->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Node n = mpDirectedGraph->raw().nodeFromId( graphElementId );
    bool status = raw().status(n);
    return status;
}

bool DirectedSubGraph::enabled(Edge::Ptr edge) const
{
    GraphElementId graphElementId = edge->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Arc arc = mpDirectedGraph->raw().arcFromId( graphElementId );
    return raw().status(arc);
}

} // end namespace lemon
} // end namespace graph_analysis
