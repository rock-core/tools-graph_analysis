#include "DirectedSubGraph.hpp"
#include "DirectedGraph.hpp"

namespace graph_analysis {
namespace lemon {

DirectedSubGraph::DirectedSubGraph(const DirectedGraph::Ptr& graph)
    : SubGraphImpl(graph)
    , mpDirectedGraph(graph)
    , mNodeMap(graph->raw())
    , mArcMap(graph->raw())
{
    // graph_t refers to the given subgraph
    // needs to be explicitly set
    setSubgraph( new graph_t(mpDirectedGraph->raw(), mNodeMap, mArcMap) );
}

void DirectedSubGraph::enable(const Vertex::Ptr& vertex)
{
    GraphElementId graphElementId = vertex->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Node n = mpDirectedGraph->raw().nodeFromId( graphElementId );

    raw().enable(n);
}

void DirectedSubGraph::disable(const Vertex::Ptr& vertex)
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

void DirectedSubGraph::enable(const Edge::Ptr& edge)
{
    GraphElementId graphElementId = edge->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Arc arc = mpDirectedGraph->raw().arcFromId( graphElementId );

    raw().enable(arc);
}

void DirectedSubGraph::disable(const Edge::Ptr& edge)
{
    GraphElementId graphElementId = edge->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Arc arc = mpDirectedGraph->raw().arcFromId( graphElementId );

    raw().disable(arc);
}

bool DirectedSubGraph::enabled(const Vertex::Ptr& vertex) const
{
    GraphElementId graphElementId = vertex->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Node n = mpDirectedGraph->raw().nodeFromId( graphElementId );
    bool status = raw().status(n);
    return status;
}

bool DirectedSubGraph::enabled(const Edge::Ptr& edge) const
{
    GraphElementId graphElementId = edge->getId( mpDirectedGraph->getId() );
    ::lemon::ListDigraph::Arc arc = mpDirectedGraph->raw().arcFromId( graphElementId );
    return raw().status(arc);
}

} // end namespace lemon
} // end namespace graph_analysis
