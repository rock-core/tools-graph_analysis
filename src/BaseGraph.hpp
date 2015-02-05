#ifndef GRAPH_ANALYSIS_BASE_GRAPH_HPP
#define GRAPH_ANALYSIS_BASE_GRAPH_HPP

#include <stdint.h>
#include <graph_analysis/EdgeIterator.hpp>
#include <graph_analysis/VertexIterator.hpp>
#include <graph_analysis/Filter.hpp>

#include <boost/shared_ptr.hpp>

/**
 * The main namespace of this library
 */
namespace graph_analysis
{

/**
 * \brief General Graph template that should be implemented to wrap specific graph library
 * functionality
 * We assume that all graph implementations will rely and allow access to nodes and edges via
 * integers
 */
class BaseGraph : public VertexIterable, public EdgeIterable
{
public:

    typedef boost::shared_ptr<BaseGraph> Ptr;

    BaseGraph()
        : mId(msId++)
    {}

    /**
     * \brief Default deconstructor
     */
    virtual ~BaseGraph() {}

    /**
     * \brief Add a vertex
     * \details
     * The base implementation does not add the vertex to the graph, but checks only if the
     * vertex is already associated with the graph.
     * Reimplement for the specific graph implementation and call the base function first, i.e.
     * BaseGraph::addVertex(v)
     * \throws std::runtime_error if the vertex already exists in the graph
     * \returns GraphElementId which identifies the vertex in this(!) graph
     * instance
     */
    virtual GraphElementId addVertex(Vertex::Ptr vertex);

    /**
     * \brief Get the vertex id for this graph
     * \throw std::runtime_error if the vertex is not part of this graph
     */
    GraphElementId getVertexId(Vertex::Ptr vertex) const { return boost::dynamic_pointer_cast<GraphElement>(vertex)->getId( getId() ); }

    /**
     * \brief Get the vertex by id
     * \return vertex
     */
    virtual Vertex::Ptr getVertex(GraphElementId id) const = 0;

    /**
     * \brief Remove vertex
     * In order to reimplement, call the base function first
     * BaseGraph::addVertex(v)
     */
    virtual void removeVertex(Vertex::Ptr vertex);

    /**
     * \brief Add an edge
     * Cannot be reimplemented directly, but see addEdge(Edge::Ptr, GraphElementId, GraphElementId)
     */
    GraphElementId addEdge(Edge::Ptr edge);

    /**
     * Remove an edge and disassociate from this graph
     * In order to reimplement, call the base function first
     * BaseGraph::addVertex(v)
     */
    virtual void removeEdge(Edge::Ptr edge);

    /**
     * Test if graph contains this edge
     * \return true if graph contains edge, false otherwise
     */
    bool contains(Edge::Ptr edge) const;

    /**
     * Test if graph contains this vertex
     * \return true if graph contains vertex, false otherwise
     */
    bool contains(Vertex::Ptr vertex) const;

    /**
     * \brief Get the edge id for this graph
     * \throw std::runtime_error if the edge is not part of this graph
     */
    GraphElementId getEdgeId(Edge::Ptr edge) const { return boost::dynamic_pointer_cast<GraphElement>(edge)->getId( getId() ); }

    /**
     * \brief Get the edge by id
     * \return vertex
     */
    virtual Edge::Ptr getEdge(GraphElementId id) const = 0;

    /**
     * \brief Get edge by given vertices
     * \return vertex
     */
    virtual std::vector<Edge::Ptr> getEdges(Vertex::Ptr source, Vertex::Ptr target);

    /**
     * Get the graph id
     */
    GraphId getId() const { return mId; }

    virtual uint64_t getNodeCount() { throw std::runtime_error("BaseGraph::getNodeCount: not implemented"); }

    virtual BaseGraph::Ptr copy() { throw std::runtime_error("BaseGraph::copy: not implemented"); }

    /**
     * Allow to create an instance of the same type of graph
     */
    virtual BaseGraph::Ptr cleanCopy() { throw std::runtime_error("BaseGraph::createInstance: not implemented"); }

    /**
     * Get the iterator over all vertices in this graph
     * \return the vertex iterator
     */
    virtual VertexIterator::Ptr getVertexIterator() = 0;

    /**
     * Get iterator over all edges in this graph
     * \return the edge iterator
     */
    virtual EdgeIterator::Ptr getEdgeIterator() = 0;

    /**
     * Retrieve all vertices
     * This default implementation uses the iterator, thus requires O(|vertices|)
     * \return all vertices in the graph
     */
    virtual std::vector<Vertex::Ptr> getAllVertices();

    /**
     * Retrieve all edges
     * The default implementation uses the iterator, thus requires O(|edges|)
     * \return all edges in the graph
     */
    virtual std::vector<Edge::Ptr> getAllEdges();

    /**
     * Get iterator over all edges (in/out) from the given vertex
     * \return EdgeIterator over all edges of this vertex
     */
    virtual EdgeIterator::Ptr getEdgeIterator(Vertex::Ptr vertex) { throw std::runtime_error("BaseGraph::getEdgeIterator: not implemented"); }

protected:
    /**
     * Add an add using source and target vertex on the internal
     * graph representation
     * \return Element id of this node within this graph
     */
    virtual GraphElementId addEdgeInternal(Edge::Ptr edge, GraphElementId sourceVertexId, GraphElementId edgeVertexId) = 0;


private:
    GraphId mId;
    static GraphId msId;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_BASE_GRAPH_HPP
