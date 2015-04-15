#ifndef GRAPH_ANALYSIS_BASE_GRAPH_HPP
#define GRAPH_ANALYSIS_BASE_GRAPH_HPP

#include <stdint.h>
#include <boost/shared_ptr.hpp>
#include <graph_analysis/EdgeIterator.hpp>
#include <graph_analysis/VertexIterator.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/SubGraph.hpp>
#include <graph_analysis/Algorithms.hpp>

/**
 * The main namespace of this library
 */
namespace graph_analysis
{

/**
 * \brief General Graph template that should be implemented to wrap specific graph library
 * functionality
 * We assume that all graph implementations will rely and allow access to vertices and edges via
 * integers
 */
class BaseGraph : public VertexIterable, public EdgeIterable, public Algorithms
{
public:
    enum ImplementationType { LEMON_DIRECTED_GRAPH, SNAP_DIRECTED_GRAPH, IMPLEMENTATION_TYPE_END };
    static std::map<ImplementationType, std::string> ImplementationTypeTxt;

    typedef boost::shared_ptr<BaseGraph> Ptr;

    BaseGraph(ImplementationType type);

    /**
     * \brief Default deconstructor
     */
    virtual ~BaseGraph() {}

    static Ptr getInstance(ImplementationType type);

    static SubGraph::Ptr getSubGraph(Ptr graph);

    ImplementationType getImplementationType() const { return mImplementationType; }
    std::string getImplementationTypeName() const { return ImplementationTypeTxt[mImplementationType]; }

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
    virtual Vertex::Ptr getVertex(GraphElementId id) const { throw std::runtime_error("graph_analysis::BaseGraph::getVertex has not been implemented"); }

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
    virtual Edge::Ptr getEdge(GraphElementId id) const { throw std::runtime_error("BaseGraph::getEdge: not implemented"); }

    /**
     * \brief Get edge by given vertices
     * \return vertex
     */
    virtual std::vector<Edge::Ptr> getEdges(Vertex::Ptr source, Vertex::Ptr target);

    /**
     * \brief Get edges by given vertices and return only edges of a given type
     * Type should be a subclass of Edge!!
     * If boost::dynamic_pointer_cast returns null pointer, object is not added
     */
    template<typename T>
    std::vector< boost::shared_ptr<T> > getEdges(Vertex::Ptr source, Vertex::Ptr target)
    {
        std::vector< boost::shared_ptr<T> > edges;
        EdgeIterator::Ptr edgeIt = getEdgeIterator(source);
        while(edgeIt->next())
        {
            boost::shared_ptr<T> edge = boost::dynamic_pointer_cast<T>( edgeIt->current() );
            if(edge && edge->getTargetVertex() == target)
            {
                edges.push_back(edge);
            }
        }

        return edges;
    }

    /**
     * Get the graph id
     */
    GraphId getId() const { return mId; }

    /**
     * Create a copy of this graph
     * \return pointer to the copy of this graph
     */
    virtual BaseGraph::Ptr copy() { throw std::runtime_error("BaseGraph::copy: not implemented"); }

    /**
     * Allow to create an instance of the same type of graph
     */
    virtual BaseGraph::Ptr cleanCopy() { throw std::runtime_error("BaseGraph::createInstance: not implemented"); }

    /**
     * Get the iterator over all vertices in this graph
     * \return the vertex iterator
     */
    virtual VertexIterator::Ptr getVertexIterator() { throw std::runtime_error("graph_analysis::BaseGraph::getVertexIterator has not been implemented"); }

    /**
     * Get iterator over all edges in this graph
     * \return the edge iterator
     */
    virtual EdgeIterator::Ptr getEdgeIterator() { throw std::runtime_error("graph_analysis::BaseGraph::getEdgeIterator has not been implemented"); }

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

    /**
     * Apply filters to base graph
     * \return subgraph with filters applied
     */
    static SubGraph::Ptr applyFilters(Ptr graph, Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter);

    /**
     * Clear the graph from all nodes and edges
     */
    void clear();

protected:
    /**
     * Add an edge using source and target vertex on the internal
     * graph representation
     * \return Element id of this edge within this graph
     */
    virtual GraphElementId addEdgeInternal(Edge::Ptr edge, GraphElementId sourceVertexId, GraphElementId edgeVertexId) { throw std::runtime_error("BaseGraph::addEdgeInternal: not implemented"); }

    /**
     * Create subgraph of the given baseGraph
     * \param baseGraph BaseGraph that this subgraph is related to
     */
    virtual SubGraph::Ptr createSubGraph(Ptr baseGraph) { throw std::runtime_error("BaseGraph::createSubGraph: not implemented"); }

private:
    GraphId mId;
    static GraphId msId;
    ImplementationType mImplementationType;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_BASE_GRAPH_HPP
