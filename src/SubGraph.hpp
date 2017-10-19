#ifndef GRAPH_ANALYSIS_SUB_GRAPH_HPP
#define GRAPH_ANALYSIS_SUB_GRAPH_HPP

#include "SharedPtr.hpp"
#include "VertexIterable.hpp"
#include "EdgeIterable.hpp"
#include "Filter.hpp"

namespace graph_analysis {

class BaseGraph;

/**
 * \brief A subgraph represent a particular filtered 'view' on a BaseGraph
 * \details Initialize the Subgraph G'(V',E') with a BaseGraph G(V,E), where the following holds
 *
 \f[ V' \subseteq V \land E' \subseteq E \f]
 *
 * The subset membership for vertices and edges can be controlled via functions
 * SubGraph::enable and SubGraph::disable
 *
 * The iterators of the subgraph check the membership in order to transparently
 * skip vertices / edges that are not members of the subgraph
 *
 * Disabling a vertex will result in disabling all outgoing or incoming edges
 * Reenabling such vertex however will only reenable the particular vertex
 *
 * In order to export the SubGraph as a BaseGraph use SubGraph::toBaseGraph
 *
 */
class SubGraph : public VertexIterable, public EdgeIterable
{
    /// Ptr which refers to the original (super) graph
    shared_ptr<BaseGraph> mpBaseGraph;

public:
    /**
     * Default constructor
     * \param graph Initialize the Subgraph G'(V',E') with a BaseGraph G(V,E), where the following holds
     *
     \f[ V' \subseteq V \land E' \subseteq E \f]
     */
    SubGraph(const shared_ptr<BaseGraph>& graph);

    virtual ~SubGraph() {}

    typedef shared_ptr<SubGraph> Ptr;

    /**
     * Enable the given vertex
     * \param vertex Vertex that is part of the underlying main graph and should
     * also be part of the subgraph
     */
    virtual void enable(const Vertex::Ptr& vertex) { (void)vertex; throw std::runtime_error("graph_analysis::SubGraph::enable(Vertex::Ptr) not implemented"); }

    /**
     * Disable the given vertex
     * \param vertex Vertex that is part of the underlying main graph and should
     * not(!) be part of the subgraph
     */
    virtual void disable(const Vertex::Ptr& vertex) { (void)vertex; throw std::runtime_error("graph_analysis::SubGraph::disable(Vertex::Ptr) not implemented"); }

    /**
     * Enable the given edge
     * \param edge Edge that is part of the underlying main graph and should
     * be part of the subgraph
     */
    virtual void enable(const Edge::Ptr& edge) { (void)edge; throw std::runtime_error("graph_analysis::SubGraph::enable(Edge::Ptr) not implemented"); }

    /**
     * Disable the given edge
     * \param edge Edge that is part of the underlying main graph and should
     * not be part of the subgraph
     */
    virtual void disable(const Edge::Ptr& edge) { (void)edge; throw std::runtime_error("graph_analysis::SubGraph::disable(Edge::Ptr) not implemented"); }

    /**
     * Test if a vertex is enabled
     * \return True if vertex is enabled, false otherwise
     */
    virtual bool enabled(const Vertex::Ptr& vertex) const { (void) vertex; throw std::runtime_error("graph_analysis::SubGraph::enabled(Vertex::Ptr) not implemented"); }

    /**
     * Test if an edge is enabled
     * \return True if edge is enabled, false otherwise
     */
    virtual bool enabled(const Edge::Ptr& edge) const { (void) edge; throw std::runtime_error("graph_analysis::SubGraph::enabled(Edge::Ptr) not implemented"); }

    /**
     * Test if a vertex is disabled
     * \return True if vertex is disable, false otherwise
     */
    bool disabled(const Vertex::Ptr& vertex) const { return !enabled(vertex); }

    /**
     * Test if an edge is disabled
     * \return True if edge is disabled, false otherwise
     */
    bool disabled(const Edge::Ptr& edge) const { return !enabled(edge); }

    /**
     * Enable all vertices
     */
    void enableAllVertices();

    /**
     * Enable all edges
     */
    void enableAllEdges();

    /**
     * Disable all vertices
     */
    void disableAllVertices();

    /**
     * Disable all edges
     */
    void disableAllEdges();

    /**
     * Apply filters to this subgraph
     */
    void applyFilters(const Filter<Vertex::Ptr>::Ptr& vertexFilter, const Filter<Edge::Ptr>::Ptr& edgeFilter);

    /**
     * Convert the subgraph into a base graph by copying only the enabled nodes
     * and edges
     */
    virtual shared_ptr<BaseGraph> toBaseGraph() const;

    /**
     * Get the iterator over all vertices in this subgraph
     * \return the egde iterator
     */
    virtual VertexIterator::Ptr getVertexIterator() const;

    /**
     * Get iterator over all edge in this subgraph
     * \return the edge iterator
     */
    virtual EdgeIterator::Ptr getEdgeIterator() const;

    /**
     * Get iterator over all edges that are starting a vertex
     * \return the edge iterator
     */
    virtual EdgeIterator::Ptr getEdgeIterator(const Vertex::Ptr& vertex) const;

    /**
     * Get the underlying base graph
     */
    shared_ptr<BaseGraph> getBaseGraph() const;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SUB_GRAPH_HPP
