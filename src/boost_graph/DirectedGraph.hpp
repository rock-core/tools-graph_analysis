#ifndef GRAPH_ANALYSIS_BOOST_GRAPH_DIRECTED_GRAPH_HPP
#define GRAPH_ANALYSIS_BOOST_GRAPH_DIRECTED_GRAPH_HPP

#include <graph_analysis/Graph.hpp>
#include <graph_analysis/SubGraphImpl.hpp>
#include <graph_analysis/DirectedGraphInterface.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <base/Logging.hpp>
#include <graph_analysis/boost_graph/ArcIterator.hpp>

#include <map>

namespace graph_analysis {
/// no using boost namespace here to avoid collisions
namespace boost_graph {

//typedef boost::undirectedS GType;
//typedef boost::directedS GType;
typedef boost::bidirectionalS GType;

typedef boost::property< boost::vertex_index_t, std::size_t, Vertex::Ptr> VProp;
typedef boost::listS VRep;
typedef boost::listS ERep;
typedef boost::property< boost::vertex_index_t, std::size_t, Vertex::Ptr> VProp;
typedef boost::property< boost::edge_index_t, std::size_t, Edge::Ptr> EProp;
typedef boost::adjacency_list<VRep, ERep, GType, VProp, EProp> AdjacencyGraph;

typedef boost::graph_traits<AdjacencyGraph>::vertex_descriptor VertexDescriptor;
typedef boost::graph_traits<AdjacencyGraph>::vertex_iterator VertexIteratorImpl;
typedef std::pair<VertexIterator, VertexIterator> VertexRange;

typedef boost::graph_traits<AdjacencyGraph>::edge_descriptor EdgeDescriptor;
typedef boost::graph_traits<AdjacencyGraph>::edge_iterator EdgeIteratorImpl;
typedef std::pair<EdgeIterator, EdgeIterator> EdgeRange;

typedef boost::graph_traits<AdjacencyGraph>::adjacency_iterator AdjacencyIterator;
typedef std::pair<AdjacencyIterator, AdjacencyIterator> AdjacencyRange;

class DirectedGraph : public TypedGraph< AdjacencyGraph >
{
    friend class OutArcIterator<DirectedGraph>;
    friend class InArcIterator<DirectedGraph>;
public:
    DirectedGraph();
    virtual ~DirectedGraph();

    BaseGraph::Ptr copy() const;
    BaseGraph::Ptr newInstance() const;

    typedef std::map<GraphElementId, VertexDescriptor> VertexMap;
    typedef std::map<GraphElementId, EdgeDescriptor> EdgeMap;

    /**
     * \brief Add a vertex
     * \return the created vertex
     */
    virtual GraphElementId addVertex(Vertex::Ptr vertex);

    /**
     * \brief Add an edge
     * \return the created edge
     */
    GraphElementId addEdgeInternal(Edge::Ptr edge, GraphElementId sourceVertexId, GraphElementId targetVertexId);

    Vertex::Ptr getVertex(GraphElementId id) const;
    Edge::Ptr getEdge(GraphElementId id) const;

    void removeVertex(Vertex::Ptr vertex);
    void removeEdge(Edge::Ptr edge);

    DirectedGraph(const DirectedGraph& other);

    void write(std::ostream& ostream = std::cout) const;

    /**
     * Get the vertex iterator for this implementation
     */
    VertexIterator::Ptr getVertexIterator() const;

    /**
     * Get the edge iterator for this implementation
     */
    EdgeIterator::Ptr getEdgeIterator() const;

    /**
     * Get EdgeIterator
     */
    EdgeIterator::Ptr getEdgeIterator(Vertex::Ptr vertex) const;

    //graph_analysis::EdgeIterator::Ptr getOutEdgeIterator(Vertex::Ptr vertex);
    //graph_analysis::EdgeIterator::Ptr getInEdgeIterator(Vertex::Ptr vertex);

    /**
     * Identifies the connected components
     */
    SubGraph::Ptr identifyConnectedComponents();

    /**
     * Get the subgraph -- by default all vertices and edges of the
     * base graph are available (enabled)
     */
    SubGraph::Ptr createSubGraph(BaseGraph::Ptr baseGraph) const;

protected:
    // Property maps to store data associated with vertices and edges
    EdgeMap mEdgeMap;
    VertexMap mVertexMap;
    static GraphElementId msNewVertexId;
    static GraphElementId msNewEdgeId;
};

} // end namespace boost_graph
} // end namespace graph_analysis


//EdgeList result;
//EdgeRange range = boost::edges(mGraph);
//for(EdgeIterator it = range.first; it != range.second; it++)
//{
//        if(type == 0 || mGraph[*it].edge_type == type)
//        {
//                mapInsert(it, result);
//        }
//}
//return result;
#endif // GRAPH_ANALYSIS_BOOST_DIRECTED_GRAPH_HPP
