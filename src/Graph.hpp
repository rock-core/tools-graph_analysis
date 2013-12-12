#ifndef GRAPH_ANALYSIS_GRAPH_HPP
#define GRAPH_ANALYSIS_GRAPH_HPP

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
 * We assume than all graph implementation will rely and allow access to nodes and edges via
 * integer
 */
class BaseGraph
{
public:
    BaseGraph()
        : mId(msId++)
    {}

    /**
     * \brief Default deconstructor
     */
    virtual ~BaseGraph() {}

    /**
     * \brief Add a vertex
     * In order to reimplement, call the base function first
     * BaseGraph::addVertex(v)
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
     * Get the graph id
     */
    GraphId getId() const { return mId; }

    /**
     * Get the vertex iterator
     */
    virtual VertexIterator::Ptr getVertexIterator() = 0;

    /**
     * Get the node iterator
     */
    virtual EdgeIterator::Ptr getEdgeIterator() = 0;

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

template<typename T>
class TypedGraph : public BaseGraph
{
public:
    TypedGraph()
        : mGraph()
    {}

    typedef T GraphType;

    /**
     * Return underlying raw graph instance
     */
    GraphType& raw() { return mGraph; }

protected:
    GraphType mGraph;

};

/**
 * \brief TypedSubGraph represent a wrapper for libraries that use an invidual datatype for subgraphs
 * \details This wrapper guarantees that vertex filters and edgefilters won't be deallocated
 * as long as the subgraph is in use, e.g., for the lemon library the subgraph holds a reference to the main graph and only
 * associates filters -- but since the filters are added as references they need to be managed properly.
 * \tparam SubGraphType the underlying subgraph type
 * \tparam VertexFilter the vertex filter
 * \tparam EdgeFilter the edge filter
 */
template<typename SubGraphType, typename VertexFilter, typename EdgeFilter>
class TypedSubGraph
{
public:
    typedef SubGraphType GraphType;
    typedef TypedSubGraph<SubGraphType, VertexFilter, EdgeFilter> BaseSubGraphType;

    typedef VertexFilter VertexFilterType;
    typedef EdgeFilter EdgeFilterType;

    /**
     * This default Constructor will not(!) initialize the underlying subgraph type, but
     * only the given filters.
     * Make sure that any class inheriting from this one properly initalizes the subgraph, i.e.
     * using setSubgraph
     *
     * Ownership of all objects, i.e. filters and graph is transferred to this class, which internally
     * uses shared pointer to guarantee cleanup when necessary
     */
    TypedSubGraph(VertexFilter* vertexFilter, EdgeFilter* edgeFilter)
    {
        mpVertexFilter = boost::shared_ptr<VertexFilter>(vertexFilter);
        mpEdgeFilter = boost::shared_ptr<EdgeFilter>(edgeFilter);
    }

    /**
     * Retrieve the raw subgraph type. Use with care since this object's memory might
     * be deallocated if you release your main object
     */
    SubGraphType& raw()
    {
        assert(mpSubgraph);
        return (*mpSubgraph);
    }

protected:
    /**
     * Get vertex filter
     * \return reference to vertex filter
     */
    VertexFilter& getVertexFilter() const
    {
        assert(mpVertexFilter);
        return *mpVertexFilter.get();
    }

    /**
     * Get edge filter
     * \return reference to edge filter
     */
    EdgeFilter& getEdgeFilter() const
    {
        assert(mpEdgeFilter);
        return *mpEdgeFilter.get();
    }

    /**
     * Set the subgraph
     * \param pointer to subgraph
     */
    void setSubgraph(SubGraphType* subgraph)
    {
        assert(subgraph);
        mpSubgraph = boost::shared_ptr<SubGraphType>(subgraph);
    }

    boost::shared_ptr<VertexFilter> mpVertexFilter;
    boost::shared_ptr<EdgeFilter> mpEdgeFilter;

    boost::shared_ptr<SubGraphType> mpSubgraph;
};


}
#endif // GRAPH_ANALYSIS_GRAPH_HPP
