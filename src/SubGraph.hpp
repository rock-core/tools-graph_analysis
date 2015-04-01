#ifndef GRAPH_ANALYSIS_SUB_GRAPH_HPP
#define GRAPH_ANALYSIS_SUB_GRAPH_HPP

#include <boost/shared_ptr.hpp>
#include <graph_analysis/VertexIterator.hpp>
#include <graph_analysis/EdgeIterator.hpp>
#include <graph_analysis/Filter.hpp>

namespace graph_analysis {

class BaseGraph;

class SubGraph : public VertexIterable, public EdgeIterable
{
    BaseGraph* mpBaseGraph;

public:
    SubGraph(BaseGraph* graph);

    typedef boost::shared_ptr<SubGraph> Ptr;

    /**
     * Enable the given vertex
     * \param vertex Vertex that is part of the underlying main graph and should
     * also be part of the subgraph
     */
    virtual void enable(Vertex::Ptr vertex) { throw std::runtime_error("graph_analysis::SubGraph::enable(Vertex::Ptr) not implemented"); }

    /**
     * Disable the given vertex
     * \param vertex Vertex that is part of the underlying main graph and should
     * not(!) be part of the subgraph
     */
    virtual void disable(Vertex::Ptr vertex) { throw std::runtime_error("graph_analysis::SubGraph::disable(Vertex::Ptr) not implemented"); }

    /**
     * Enable the given edge
     * \param edge Edge that is part of the underlying main graph and should
     * be part of the subgraph
     */
    virtual void enable(Edge::Ptr edge) { throw std::runtime_error("graph_analysis::SubGraph::enable(Edge::Ptr) not implemented"); }

    /**
     * Disable the given edge
     * \param edge Edge that is part of the underlying main graph and should
     * not be part of the subgraph
     */
    virtual void disable(Edge::Ptr edge) { throw std::runtime_error("graph_analysis::SubGraph::disable(Edge::Ptr) not implemented"); }

    /**
     * Test if a vertex is enabled
     * \return True if vertex is enabled, false otherwise
     */
    virtual bool enabled(Vertex::Ptr vertex) const { throw std::runtime_error("graph_analysis::SubGraph::enabled(Vertex::Ptr) not implemented"); }

    /**
     * Test if an edge is enabled
     * \return True if edge is enabled, false otherwise
     */
    virtual bool enabled(Edge::Ptr edge) const { throw std::runtime_error("graph_analysis::SubGraph::enabled(Edge::Ptr) not implemented"); }

    /**
     * Test if a vertex is disabled
     * \return True if vertex is disable, false otherwise
     */
    bool disabled(Vertex::Ptr vertex) const { return !enabled(vertex); }

    /**
     * Test if an edge is disabled
     * \return True if edge is disabled, false otherwise
     */
    bool disabled(Edge::Ptr edge) const { return !enabled(edge); }

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
    void applyFilters(Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter);

    /**
     * Convert the subgraph into a base graph by copying only the enabled nodes
     * and edges
     */
    virtual boost::shared_ptr<BaseGraph> toBaseGraph();

    /**
     * Get the iterator over all vertices in this subgraph
     * \return the egde iterator
     */
    virtual VertexIterator::Ptr getVertexIterator();

    /**
     * Get iterator over all edge in this subgraph
     * \return the edge iterator
     */
    virtual EdgeIterator::Ptr getEdgeIterator();

protected:
    BaseGraph* getBaseGraph();
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SUB_GRAPH_HPP
