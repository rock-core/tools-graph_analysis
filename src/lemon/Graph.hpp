#ifndef GRAPH_ANALYSIS_LEMON_GRAPH_HPP
#define GRAPH_ANALYSIS_LEMON_GRAPH_HPP

#include <lemon/list_graph.h>
#include <lemon/adaptors.h>
#include <lemon/lgf_writer.h>
#include <lemon/connectivity.h>

#include <graph_analysis/Graph.hpp>
#include <graph_analysis/DirectedGraphInterface.hpp>
#include <graph_analysis/lemon/NodeIterator.hpp>
#include <graph_analysis/lemon/ArcIterator.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace lemon {

class DirectedGraph;

typedef ::lemon::SubDigraph< ::lemon::ListDigraph, ::lemon::ListDigraph::NodeMap<bool>, ::lemon::ListDigraph::ArcMap<bool> > SubGraph;

/**
 * A subgraph provide a subset of an existing graph
 */
class DirectedSubGraph : public TypedSubGraph< SubGraph, ::lemon::ListDigraph::NodeMap<bool>, ::lemon::ListDigraph::ArcMap<bool> >
{
    DirectedGraph& mGraph;
public:
    DirectedSubGraph(DirectedGraph& graph);

    void enable(Vertex::Ptr vertex);
    void disable(Vertex::Ptr vertex);

    void enable(Edge::Ptr edge);
    void disable(Edge::Ptr edge);

    bool enabled(Vertex::Ptr vertex) const;
    bool enabled(Edge::Ptr vertex) const;
};

/**
 * \class DirectedGraph
 * \brief Directed graph implementation based on lemon library
 */
class DirectedGraph : public TypedGraph< ::lemon::ListDigraph >, graph_analysis::DirectedGraphInterface
{
public:
    /**
     * \brief Default constructor of the graph
     */
    DirectedGraph();

    /**
     * Copy the graph
     */
    BaseGraph::Ptr copy();

    /**
     * Copy the graph
     */
    BaseGraph::Ptr cleanCopy();

    typedef GraphType::ArcMap< Edge::Ptr > EdgeMap;
    typedef GraphType::NodeMap< Vertex::Ptr > VertexMap;

    typedef GraphType::NodeMap<bool> VertexActivationMap;
    typedef GraphType::ArcMap<bool> EdgeActivationMap;

    typedef GraphType::ArcMap< std::string > EdgeStringMap;
    typedef GraphType::NodeMap< std::string > VertexStringMap;

    typedef GraphType::ArcMap< GraphElementId > EdgeIdMap;
    typedef GraphType::NodeMap< GraphElementId > VertexIdMap;

    // Register the DirecteSubGraph as official SubGraph
    typedef DirectedSubGraph SubGraph;

    friend class NodeIterator<DirectedGraph>;
    friend class ArcIterator<DirectedGraph>;
    friend class InArcIterator<DirectedGraph>;
    friend class OutArcIterator<DirectedGraph>;

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

    /**
     * \brief Get the source vertex for this edge
     * \return Pointer to the vertex data
     */
    Vertex::Ptr getSourceVertex(Edge::Ptr e) const;

    /**
     * \brief Get the target vertex for this edge
     * \return Pointer to the vertex data
     */
    Vertex::Ptr getTargetVertex(const Edge::Ptr& e) const;

    void removeVertex(Vertex::Ptr vertex);
    void removeEdge(Edge::Ptr edge);

    DirectedGraph(const DirectedGraph& other);

    /**
     * \brief Direct usage off operator= is disallowed in lemon, thus
     * need for explicit usage of copy functions
     */
    DirectedGraph& operator=(const DirectedGraph& other);

    /**
     * Filters in this context are permissive, i.e. they mark what to show
     */
    DirectedSubGraph applyFilters(Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter);

    void write(std::ostream& ostream = std::cout) const;

    /**
     * Get the vertex iterator for this implementation
     */
    VertexIterator::Ptr getVertexIterator();

    /**
     * Get the edge iterator for this implementation
     */
    EdgeIterator::Ptr getEdgeIterator();

    EdgeIterator::Ptr getEdgeIterator(Vertex::Ptr vertex);

    EdgeIterator::Ptr getOutEdgeIterator(Vertex::Ptr vertex);
    EdgeIterator::Ptr getInEdgeIterator(Vertex::Ptr vertex);

    DirectedGraph::SubGraph identifyConnectedComponents(DirectedGraph& graph, DirectedGraph::SubGraph& subgraph);

    uint64_t getNodeCount();

protected:
    // Property maps to store data associated with vertices and edges
    EdgeMap mEdgeMap;
    VertexMap mVertexMap;
};


} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_GRAPH_HPP
