#include <sstream>
#include "DirectedSubGraph.hpp"
#include "DirectedGraph.hpp"
#include "../BaseGraph.hpp"

#include "ArcIterator.hpp"
#include "NodeIterator.hpp"
#include "../filters/CommonFilters.hpp"

#include <boost/graph/incremental_components.hpp>
#include <boost/pending/disjoint_sets.hpp>
#include <boost/foreach.hpp>

namespace graph_analysis {
namespace boost_graph {

std::map<GraphId, GraphElementId> DirectedGraph::msNewVertexId;
std::map<GraphId, GraphElementId> DirectedGraph::msNewEdgeId;

BaseGraph::Ptr DirectedGraph::copy() const
{
    return BaseGraph::Ptr(new DirectedGraph(*this));
}

BaseGraph::Ptr DirectedGraph::newInstance() const
{
    return BaseGraph::Ptr(new DirectedGraph());
}

DirectedGraph::DirectedGraph()
    : TypedGraph(BOOST_DIRECTED_GRAPH, true)
{
}

DirectedGraph::DirectedGraph(const DirectedGraph& other)
    : TypedGraph(BOOST_DIRECTED_GRAPH, true)
{
    *this = other;
}

DirectedGraph::~DirectedGraph()
{
}

DirectedGraph::Ptr DirectedGraph::validateType(const BaseGraph::Ptr& baseGraph) const
{
    const DirectedGraph::Ptr& directedGraph =
        dynamic_pointer_cast<boost_graph::DirectedGraph>(baseGraph);
    if(!directedGraph)
    {
        throw std::invalid_argument("graph_analysis::boost_graph::DirectedGraph::validateType: base graph could not be cast to DirectedGraph");
    }
    return directedGraph;
}

GraphElementId DirectedGraph::addVertexInternal(const Vertex::Ptr& vertex)
{
    // Add a new vertex to the Graph
    VertexDescriptor vertexDescriptor = boost::add_vertex(mGraph);
    mGraph[vertexDescriptor] = vertex;

    GraphElementId newVertexId = msNewVertexId[getId()]++;

    // Set the internal index property (This probably shouldn't be done.)
    //::boost::put( index, vertexDescriptor, newVertexId);

    // Insert to ID-Vertex map and memorize last added vertex
    mVertexMap.insert(VertexMap::value_type(newVertexId, vertexDescriptor));
    vertex->associate(getId(), newVertexId);

    return newVertexId;
}

void DirectedGraph::removeVertexInternal(const Vertex::Ptr& vertex)
{
    int vertexId = getVertexId(vertex);
    VertexDescriptor vertexDescriptor = mVertexMap[vertexId];
    mVertexMap.erase(vertexId);
    boost::clear_vertex(vertexDescriptor, mGraph);
    boost::remove_vertex(vertexDescriptor, mGraph);
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

VertexDescriptor DirectedGraph::getVertexDescriptor(const Vertex::Ptr& vertex) const
{
    GraphElementId id = getVertexId(vertex);
    VertexMap::const_iterator cit = mVertexMap.find(id);
    if(cit != mVertexMap.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("graph_analysis::boost::DirectedGraph::getVertexDescriptor for vertex " + vertex->toString() + " could no be retrieved.");
}

EdgeDescriptor DirectedGraph::getEdgeDescriptor(const Edge::Ptr& edge) const
{
    GraphElementId id = getEdgeId(edge);
    EdgeMap::const_iterator cit = mEdgeMap.find(id);
    if(cit != mEdgeMap.end())
    {
        return cit->second;
    }
    throw std::invalid_argument("graph_analysis::boost::DirectedGraph::getEdgeDescriptor for vertex " + edge->toString() + " could no be retrieved.");
}



GraphElementId DirectedGraph::addEdgeInternal(const Edge::Ptr& edge, GraphElementId sourceVertexId, GraphElementId targetVertexId)
{
    GraphElementId newEdgeId = msNewEdgeId[getId()]++;
    std::pair<EdgeDescriptor, bool> result = boost::add_edge(mVertexMap[sourceVertexId], mVertexMap[targetVertexId], mGraph);
    EdgeDescriptor edgeDescriptor = result.first;
    mGraph[edgeDescriptor] = edge;
    //::boost::put(::boost::edge_index_t(), mGraph, edgeDescriptor, newEdgeId);
    edge->associate(getId(), newEdgeId);
    mEdgeMap.insert(EdgeMap::value_type(newEdgeId, edgeDescriptor));
    return newEdgeId;
}

void DirectedGraph::removeEdgeInternal(const Edge::Ptr& edge)
{
    int edgeId = getEdgeId(edge);
    EdgeDescriptor edgeDescriptor = mEdgeMap[edgeId];
    mEdgeMap.erase(edgeId);
    boost::remove_edge(edgeDescriptor, mGraph);
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

VertexIterator::Ptr DirectedGraph::getVertexIterator() const
{
    NodeIterator<DirectedGraph>* it = new NodeIterator<DirectedGraph>(*this);
    return VertexIterator::Ptr(it);
}

EdgeIterator::Ptr DirectedGraph::getEdgeIterator() const
{
    ArcIterator<DirectedGraph>* it = new ArcIterator<DirectedGraph>(*this);
    return EdgeIterator::Ptr(it);
}

EdgeIterator::Ptr DirectedGraph::getEdgeIterator(const Vertex::Ptr& vertex) const
{
    InOutArcIterator<DirectedGraph>* it = new InOutArcIterator<DirectedGraph>(*this, vertex);
    return EdgeIterator::Ptr(it);
}

SubGraph::Ptr DirectedGraph::identifyConnectedComponents(const BaseGraph::Ptr& baseGraph) const
{
    DirectedGraph::Ptr directedGraph = validateType(baseGraph);
    SubGraph::Ptr subgraph = make_shared<SubGraph>(directedGraph);
    subgraph->disableAllVertices();

    size_t numberOfVertices = boost::num_vertices(directedGraph->raw());
    std::vector<VertexIndex> rank(numberOfVertices);
    std::vector<VertexDescriptor> parent(numberOfVertices);


    boost::disjoint_sets< VertexIndex*, VertexDescriptor* > ds(&rank[0], &parent[0]);
    boost::initialize_incremental_components(mGraph, ds);
    boost::incremental_components(mGraph, ds);

    std::vector<VertexDescriptor>::const_iterator cit = parent.begin();
    for(; cit != parent.end(); ++cit)
    {
        Vertex::Ptr v = mGraph[*cit];
        subgraph->enable(v);
    }
    return subgraph;
}

SubGraph::Ptr DirectedGraph::createSubGraph(const BaseGraph::Ptr& baseGraph) const
{
    DirectedGraph::Ptr directedGraph = validateType(baseGraph);
    SubGraph::Ptr subgraph = make_shared<boost_graph::DirectedSubGraph>(directedGraph);
    Filter< Vertex::Ptr >::Ptr vertexFilter(new filters::PermitAll< Vertex::Ptr >() );
    Filter< Edge::Ptr >::Ptr edgeFilter(new filters::PermitAll< Edge::Ptr >() );
    subgraph->applyFilters(vertexFilter, edgeFilter);
    return subgraph;
}

graph_analysis::EdgeIterator::Ptr DirectedGraph::getOutEdgeIterator(const Vertex::Ptr& vertex) const
{
    OutArcIterator<DirectedGraph>* it = new OutArcIterator<DirectedGraph>(*this, vertex);
    return EdgeIterator::Ptr(it);
}

graph_analysis::EdgeIterator::Ptr DirectedGraph::getInEdgeIterator(const Vertex::Ptr& vertex) const
{
    InArcIterator<DirectedGraph>* it = new InArcIterator<DirectedGraph>(*this, vertex);
    return EdgeIterator::Ptr(it);
}

std::vector<Edge::Ptr> DirectedGraph::getEdges(VertexDescriptor source, VertexDescriptor target) const
{
    std::vector< shared_ptr<Edge> > edges;

    typedef boost::graph_traits< DirectedGraph::graph_t >::in_edge_iterator BoostEdgeIterator;
    BoostEdgeIterator ei, edgeEnd;

    for(boost::tie(ei, edgeEnd) = boost::in_edges(target, raw());
            ei != edgeEnd; ++ei)
    {
        VertexDescriptor sourceVertexDescriptor = boost::source(*ei, raw());
        if(sourceVertexDescriptor == source)
        {
            edges.push_back(raw()[*ei]);
        }
    }
    return edges;
}

std::vector<Edge::Ptr> DirectedGraph::getEdges(const Vertex::Ptr& source, const Vertex::Ptr& target) const
{
    VertexDescriptor sourceVertexDescriptor = getVertexDescriptor(source);
    VertexDescriptor targetVertexDescriptor = getVertexDescriptor(target);

    return getEdges(sourceVertexDescriptor, targetVertexDescriptor);
}



} // end namespace boost_graph
} // endn namespace graph_analysis
