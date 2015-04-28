#include "DirectedGraph.hpp"
#include <base/Logging.hpp>

namespace graph_analysis {
namespace snap {

DirectedGraph::DirectedGraph()
    : TypedGraph<SnapDigraph>(BaseGraph::SNAP_DIRECTED_GRAPH)
{
}

DirectedGraph::~DirectedGraph()
{}

BaseGraph::Ptr DirectedGraph::copy() const
{
    // TODO: use snap internal copy
    BaseGraph::Ptr baseGraph(new DirectedGraph());

    graph_analysis::EdgeIterator::Ptr edgeIt = getEdgeIterator();
    while(edgeIt->next())
    {
        baseGraph->addEdge(edgeIt->current());
    }
    return baseGraph;
}

BaseGraph::Ptr DirectedGraph::newInstance() const
{
    return BaseGraph::Ptr(new DirectedGraph());
}

GraphElementId DirectedGraph::addVertex(Vertex::Ptr vertex)
{
    BaseGraph::addVertex(vertex);

    TInt nodeId = mGraph.AddNode();
    vertex->associate(getId(), nodeId);

    mGraph.SetNDat(nodeId, Serializable<Vertex::Ptr>(vertex));
    return nodeId;
}

void DirectedGraph::removeVertex(Vertex::Ptr vertex)
{
    int nodeId = getVertexId( vertex );
    BaseGraph::removeVertex(vertex);

    mGraph.DelNode(nodeId);
}

void DirectedGraph::removeEdge(Edge::Ptr edge)
{
    int edgeId = getEdgeId( edge );
    BaseGraph::removeEdge(edge);

    mGraph.DelEdge(edgeId);
}

/**
 * \brief Add an edge
 * \return an edge id
 */
GraphElementId DirectedGraph::addEdgeInternal(Edge::Ptr edge, GraphElementId sourceVertexId, GraphElementId targetVertexId)
{
    TInt edgeId = mGraph.AddEdge(sourceVertexId,targetVertexId);
    edge->associate(getId(), edgeId);

    mGraph.SetEDat(edgeId, Serializable<Edge::Ptr>(edge));
    return edgeId;
}

Vertex::Ptr DirectedGraph::getVertex(GraphElementId id) const
{
    return mGraph.GetNDat(id).value;
}

Edge::Ptr DirectedGraph::getEdge(GraphElementId id) const
{
    // Serializable<Edge::Ptr>.value
    return mGraph.GetEDat(id).value;
}

Vertex::Ptr DirectedGraph::getSourceVertex(Edge::Ptr e) const
{
    TInt nodeId = mGraph.GetEI( getEdgeId(e) ).GetSrcNId();
    return getVertex(nodeId);
}

Vertex::Ptr DirectedGraph::getTargetVertex(Edge::Ptr e) const
{
    TInt nodeId = mGraph.GetEI( getEdgeId(e) ).GetDstNId();
    return getVertex(nodeId);
}

/**
 * Get the vertex iterator for this implementation
 */
VertexIterator::Ptr DirectedGraph::getVertexIterator() const
{
    NodeIterator<DirectedGraph>* it = new NodeIterator<DirectedGraph>(*this);
    return VertexIterator::Ptr(it);
}

/**
 * Get the edge iterator for this implementation
 */
graph_analysis::EdgeIterator::Ptr DirectedGraph::getEdgeIterator() const
{
    EdgeIterator<DirectedGraph>* it = new EdgeIterator<DirectedGraph>(*this);
    return graph_analysis::EdgeIterator::Ptr(it);
}

graph_analysis::EdgeIterator::Ptr DirectedGraph::getEdgeIterator(Vertex::Ptr vertex) const
{
    InOutEdgeIterator<DirectedGraph>* it = new InOutEdgeIterator<DirectedGraph>(*this, vertex);
    return graph_analysis::EdgeIterator::Ptr(it);
}

graph_analysis::EdgeIterator::Ptr DirectedGraph::getOutEdgeIterator(Vertex::Ptr vertex) const
{
    OutEdgeIterator<DirectedGraph>* it = new OutEdgeIterator<DirectedGraph>(*this, vertex);
    return graph_analysis::EdgeIterator::Ptr(it);
}

graph_analysis::EdgeIterator::Ptr DirectedGraph::getInEdgeIterator(Vertex::Ptr vertex) const
{
    InEdgeIterator<DirectedGraph>* it = new InEdgeIterator<DirectedGraph>(*this, vertex);
    return graph_analysis::EdgeIterator::Ptr(it);
}

SubGraph::Ptr DirectedGraph::createSubGraph(BaseGraph::Ptr baseGraph) const
{
    // Enable all nodes and edges
    BaseGraph::Ptr graphCopy = baseGraph->copy();
    DirectedGraph::Ptr subGraph = boost::dynamic_pointer_cast<DirectedGraph>(graphCopy);
    return SubGraph::Ptr(new DirectedSubGraph(baseGraph, subGraph));
}

} // end namespace snap
} // end namespace graph_analysis
