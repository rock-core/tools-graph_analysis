#ifndef GRAPH_ANALYSIS_SNAP_DIRECTED_GRAPH_HPP
#define GRAPH_ANALYSIS_SNAP_DIRECTED_GRAPH_HPP

#include <graph_analysis/VertexIterator.hpp>
#include <graph_analysis/EdgeIterator.hpp>
#include <graph_analysis/TypedGraph.hpp>

// FIXME: when not including "sstream" here, the snap header will fail due to
// some "min"/"max" defines. this looks like a bug in their headers?
#include <sstream>

#include <snap/snap-core/Snap.h>
#include <graph_analysis/snap/DirectedSubGraph.hpp>
#include <graph_analysis/snap/NodeIterator.hpp>
#include <graph_analysis/snap/EdgeIterator.hpp>

namespace graph_analysis {
namespace snap {

template<typename T>
struct Serializable
{
    typedef T value_t;
    T value;

    Serializable()
    {}

    Serializable(T value)
        : value(value)
    {}

    void Save(TSOut& SOut) const {}
};

// TNEGraph is a DirectedGraph, but no data on nodes and edges
// PNEGraph is the Pointer to the TNEGraph
// TNodeEdgeNet is the equivalent to TNEGraph, but with data on nodes and edges
typedef TNodeEdgeNet< Serializable<Vertex::Ptr>, Serializable<Edge::Ptr> > SnapDigraph;

/**
 * \class DirectedGraph
 * \brief Directed graph implementation based on SNAP library
 * \see 
 */
class DirectedGraph : public graph_analysis::TypedGraph<SnapDigraph, BaseGraph>
{
public:
    typedef shared_ptr<DirectedGraph> Ptr;

    friend class NodeIterator<DirectedGraph>;
    friend class EdgeIterator<DirectedGraph>;
    friend class OutEdgeIterator<DirectedGraph>;
    friend class InEdgeIterator<DirectedGraph>;

    /**
     * \brief Default constructor of the graph
     */
    DirectedGraph();

    ~DirectedGraph();

    DirectedGraph(const DirectedGraph& other);

    BaseGraph::Ptr copy() const;

    BaseGraph::Ptr newInstance() const;

    Vertex::Ptr getVertex(GraphElementId id) const;
    Edge::Ptr getEdge(GraphElementId id) const;

    /**
     * \brief Get the source vertex for this edge
     * \return Pointer to the vertex data
     */
    Vertex::Ptr getSourceVertex(const Edge::Ptr& e) const;

    /**
     * \brief Get the target vertex for this edge
     * \return Pointer to the vertex data
     */
    Vertex::Ptr getTargetVertex(const Edge::Ptr& e) const;

    /**
     * Get the vertex iterator for this implementation
     */
    VertexIterator::Ptr getVertexIterator() const;

    /**
     * Get the edge iterator for this implementation
     */
    graph_analysis::EdgeIterator::Ptr getEdgeIterator() const;

    graph_analysis::EdgeIterator::Ptr getEdgeIterator(const Vertex::Ptr& vertex) const;

    graph_analysis::EdgeIterator::Ptr getOutEdgeIterator(const Vertex::Ptr& vertex) const;

    graph_analysis::EdgeIterator::Ptr getInEdgeIterator(const Vertex::Ptr& vertex) const;

protected:
    /**
     * \brief Add a vertex
     * \return the id of the newly created vertex
     */
    virtual GraphElementId addVertexInternal(const Vertex::Ptr& vertex);

    /**
     * \brief Remove vertex from the graph
     */
    virtual void removeVertexInternal(const Vertex::Ptr& vertex);

    /**
     * \brief Add an edge
     * \return an edge interator
     */
    virtual GraphElementId addEdgeInternal(const Edge::Ptr& edge, GraphElementId sourceVertexId, GraphElementId targetVertexId);

    /**
     * Remove an edge from the graph
     */
    virtual void removeEdgeInternal(const Edge::Ptr& edge);

    virtual SubGraph::Ptr createSubGraph(const BaseGraph::Ptr& baseGraph) const;
};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_DIRECTED_GRAPH_HPP
