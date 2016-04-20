#ifndef GRAPH_ANALYSIS_LEMON_DIRECTED_GRAPH_HPP
#define GRAPH_ANALYSIS_LEMON_DIRECTED_GRAPH_HPP


#include <lemon/adaptors.h>
#include <lemon/lgf_writer.h>
#include <lemon/connectivity.h>

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/lemon/DirectedSubGraph.hpp>

namespace graph_analysis {
namespace lemon {

/**
 * \class DirectedGraph
 * \brief Directed graph implementation based on lemon library
 */
class DirectedGraph : public TypedGraph< ::lemon::ListDigraph, graph_analysis::BaseGraph >
{
public:
    typedef shared_ptr<DirectedGraph> Ptr;

    /**
     * \brief Default constructor of the graph
     */
    DirectedGraph();

    ~DirectedGraph();

    /**
     * Copy the graph
     */
    BaseGraph::Ptr copy() const;

    /**
     * Create new instance of this graph
     */
    BaseGraph::Ptr newInstance() const;

    typedef graph_t::ArcMap< Edge::Ptr > EdgeMap;
    typedef graph_t::NodeMap< Vertex::Ptr > VertexMap;

    typedef graph_t::NodeMap<bool> VertexActivationMap;
    typedef graph_t::ArcMap<bool> EdgeActivationMap;

    typedef graph_t::ArcMap< std::string > EdgeStringMap;
    typedef graph_t::NodeMap< std::string > VertexStringMap;

    typedef graph_t::ArcMap< GraphElementId > EdgeIdMap;
    typedef graph_t::NodeMap< GraphElementId > VertexIdMap;

    // Register the DirectedSubGraph as subgraph type
    typedef DirectedSubGraph subgraph_t;

    friend class NodeIterator<DirectedGraph>;
    friend class ArcIterator<DirectedGraph>;
    friend class InArcIterator<DirectedGraph>;
    friend class OutArcIterator<DirectedGraph>;

    /**
     * Translate between wrapper and native formats
     */
    graph_t::Node getNode(const Vertex::Ptr& vertex) const;
    graph_t::Arc getArc(const Edge::Ptr& edge) const;

    Vertex::Ptr getVertex(graph_t::Node node) const;
    Edge::Ptr getEdge(graph_t::Arc arc) const;

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

    DirectedGraph(const DirectedGraph& other);

    /**
     * \brief Direct usage off operator= is disallowed in lemon, thus
     * need for explicit usage of copy functions
     */
    DirectedGraph& operator=(const DirectedGraph& other);

    void write(std::ostream& ostream = std::cout) const;

    /**
     * Get the vertex iterator for this implementation
     */
    VertexIterator::Ptr getVertexIterator() const;

    /**
     * Get the edge iterator for this implementation
     */
    EdgeIterator::Ptr getEdgeIterator() const;

    EdgeIterator::Ptr getEdgeIterator(const Vertex::Ptr& vertex) const;

    EdgeIterator::Ptr getOutEdgeIterator(const Vertex::Ptr& vertex) const;
    EdgeIterator::Ptr getInEdgeIterator(const Vertex::Ptr& vertex) const;

    /**
     * Identifies the connected components
     */
    SubGraph::Ptr identifyConnectedComponents(const BaseGraph::Ptr& baseGraph) const;

protected:
    /**
     * \brief Add a vertex
     * \return the created vertex
     */
    virtual GraphElementId addVertexInternal(const Vertex::Ptr& vertex);

    /**
     * Remove a vertex from the internal graph representation
     */
    virtual void removeVertexInternal(const Vertex::Ptr&);

    /**
     * \brief Add an edge
     * \return the created edge
     */
    GraphElementId addEdgeInternal(const Edge::Ptr& edge, GraphElementId sourceVertexId, GraphElementId targetVertexId);

    /**
     * Remove an edge from the internal graph representation
     * \return Element id of this vertex within this graph
     */
    virtual void removeEdgeInternal(const Edge::Ptr&);

    /**
     * Get the subgraph -- by default all vertices and edges of the
     * base graph are available (enabled)
     */
    virtual SubGraph::Ptr createSubGraph(const BaseGraph::Ptr& graph) const;

    // Property maps to store data associated with vertices and edges
    EdgeMap mEdgeMap;
    VertexMap mVertexMap;

};

} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_DIRECTED_GRAPH_HPP
