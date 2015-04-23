#include "DirectedGraph.hpp"
#include <graph_analysis/BaseGraph.hpp>
#include <sstream>

#include "ArcIterator.hpp"
#include "NodeIterator.hpp"

namespace graph_analysis {
namespace boost_graph {

GraphElementId DirectedGraph::msNewVertexId = 0;
GraphElementId DirectedGraph::msNewEdgeId = 0;

BaseGraph::Ptr DirectedGraph::copy()
{
    return BaseGraph::Ptr(new DirectedGraph(*this));
}

BaseGraph::Ptr DirectedGraph::cleanCopy()
{
    return BaseGraph::Ptr(new DirectedGraph());
}

DirectedGraph::DirectedGraph()
    : TypedGraph(BOOST_DIRECTED_GRAPH)
{
}

DirectedGraph::DirectedGraph(const DirectedGraph& other)
    : TypedGraph(BOOST_DIRECTED_GRAPH)
{
    *this = other;
}

DirectedGraph::~DirectedGraph()
{
}

GraphElementId DirectedGraph::addVertex(Vertex::Ptr vertex)
{
    BaseGraph::addVertex(vertex);

    // Add a new vertex to the Graph
    VertexDescriptor vertexDescriptor = boost::add_vertex(mGraph);
    mGraph[vertexDescriptor] = vertex;

    GraphElementId newVertexId = msNewVertexId++;
    // Set the internal index property (This probably shouldn't be done.)
    //::boost::put( boost::vertex_index_t(), mGraph, vertexDescriptor, newVertexId); 

    // Insert to ID-Vertex map and memorize last added vertex
    mVertexMap.insert(VertexMap::value_type(newVertexId, vertexDescriptor));
    vertex->associate(getId(), newVertexId);

    return newVertexId;
}

Vertex::Ptr DirectedGraph::getVertex(GraphElementId id) const
{
    VertexMap::const_iterator cit = mVertexMap.find(id);
    if(cit != mVertexMap.end())
    {
        VertexDescriptor vertexDescriptor = cit->second;
        return mGraph[ vertexDescriptor ];
    }

    std::stringstream ss;
    ss << id;
    throw std::invalid_argument("graph_analysis::boost::DirectedGraph::getVertex with id '" + ss.str() +"' does not exist");
}

GraphElementId DirectedGraph::addEdgeInternal(Edge::Ptr edge, GraphElementId sourceVertexId, GraphElementId targetVertexId)
{
    GraphElementId newEdgeId = msNewEdgeId++;
    std::pair<EdgeDescriptor, bool> result = boost::add_edge(mVertexMap[sourceVertexId], mVertexMap[targetVertexId], mGraph);
    EdgeDescriptor edgeDescriptor = result.first;
    mGraph[edgeDescriptor] = edge;
    //::boost::put(::boost::edge_index_t(), mGraph, edgeDescriptor, newEdgeId);
    edge->associate(getId(), newEdgeId);
    mEdgeMap.insert(EdgeMap::value_type(newEdgeId, edgeDescriptor));
    return newEdgeId;
}

Edge::Ptr DirectedGraph::getEdge(GraphElementId id) const
{
    EdgeMap::const_iterator cit = mEdgeMap.find(id);
    if(cit != mEdgeMap.end())
    {
        EdgeDescriptor descriptor = cit->second;
        return mGraph[descriptor];
    }

    std::stringstream ss;
    ss << id;
    throw std::invalid_argument("graph_analysis::boost::DirectedGraph::getEdge with id '" + ss.str() +"' does not exist");
}

void DirectedGraph::removeVertex(Vertex::Ptr vertex)
{
    int vertexId = getVertexId(vertex);

    BaseGraph::removeVertex(vertex);

    VertexDescriptor vertexDescriptor = mVertexMap[vertexId];
    mVertexMap.erase(vertexId);
    boost::clear_vertex(vertexDescriptor, mGraph);
    boost::remove_vertex(vertexDescriptor, mGraph);
}

void DirectedGraph::removeEdge(Edge::Ptr edge)
{
    int edgeId = getEdgeId(edge);

    BaseGraph::removeEdge(edge);
    EdgeDescriptor edgeDescriptor = mEdgeMap[edgeId];
    mEdgeMap.erase(edgeId);
    boost::remove_edge(edgeDescriptor, mGraph);
}

VertexIterator::Ptr DirectedGraph::getVertexIterator()
{
    NodeIterator<DirectedGraph>* it = new NodeIterator<DirectedGraph>(*this);
    return VertexIterator::Ptr(it);
}

EdgeIterator::Ptr DirectedGraph::getEdgeIterator()
{
    ArcIterator<DirectedGraph>* it = new ArcIterator<DirectedGraph>(*this);
    return EdgeIterator::Ptr(it);
}

EdgeIterator::Ptr DirectedGraph::getEdgeIterator(Vertex::Ptr vertex)
{
    InOutArcIterator<DirectedGraph>* it = new InOutArcIterator<DirectedGraph>(*this, vertex);
    return EdgeIterator::Ptr(it);
}

} // end namespace boost_graph
} // endn namespace graph_analysis
