#ifndef GRAPH_ANALYSIS_TYPED_SUB_GRAPH_HPP
#define GRAPH_ANALYSIS_TYPED_SUB_GRAPH_HPP

#include <assert.h>
#include <boost/shared_ptr.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {

/**
 * \brief SubGraph represent a wrapper for libraries that use an invidual datatype for subgraphs
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
     *
     * Classes that inherit from TypedSubGraph need to be explictly call
     * setSubgraph to link to the underlying SubGraph implementation
     */
    TypedSubGraph(BaseGraph& graph, VertexFilter* vertexFilter, EdgeFilter* edgeFilter)
        : mBaseGraph(graph)
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

    const SubGraphType& raw() const
    {
        assert(mpSubgraph);
        return (*mpSubgraph);
    }

    /**
     * Enable the given vertex
     * \param vertex Vertex that is part of the underlying main graph and should
     * also be part of the subgraph
     */
    virtual void enable(Vertex::Ptr vertex) = 0;

    /**
     * Disable the given vertex
     * \param vertex Vertex that is part of the underlying main graph and should
     * not(!) be part of the subgraph
     */
    virtual void disable(Vertex::Ptr vertex) = 0;

    /**
     * Enable the given edge
     * \param edge Edge that is part of the underlying main graph and should
     * be part of the subgraph
     */
    virtual void enable(Edge::Ptr edge) = 0;

    /**
     * Disable the given edge
     * \param edge Edge that is part of the underlying main graph and should
     * not be part of the subgraph
     */
    virtual void disable(Edge::Ptr edge) = 0;

    /**
     * Test if a vertex is enabled
     * \return True if vertex is enabled, false otherwise
     */
    virtual bool enabled(Vertex::Ptr vertex) const = 0;

    /**
     * Test if an edge is enabled
     * \return True if edge is enabled, false otherwise
     */
    virtual bool enabled(Edge::Ptr vertex) const = 0;

    /**
     * Apply filters to this subgraph
     */
    void applyFilters(Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter)
    {
        if(edgeFilter)
        {
            EdgeContextFilter::Ptr contextFilter = boost::dynamic_pointer_cast<EdgeContextFilter>(edgeFilter);

            EdgeIterator::Ptr edgeIterator = mBaseGraph.getEdgeIterator();

            while(edgeIterator->next())
            {
                // By default edges are disabled
                Edge::Ptr edge = edgeIterator->current();
                if( edgeFilter->permits(edge) )
                {
                    // A context filter should apply to source / target nodes -- no need to filter this edge specifically then
                    if(contextFilter)
                    {
                        bool filterTarget = contextFilter->permitsTarget(edge);
                        bool filterSource = contextFilter->permitsSource(edge);

                        if(filterSource && filterTarget)
                        {
                            enable(edge);
                        }
                    }
                }
            }
        }

        if(vertexFilter)
        {
            VertexIterator::Ptr vertexIterator = mBaseGraph.getVertexIterator();
            while(vertexIterator->next())
            {
                Vertex::Ptr vertex = vertexIterator->current();
                if( vertexFilter->permits(vertex) )
                {
                    enable(vertex);
                }
            }
        }
    }

protected:

    BaseGraph& getBaseGraph() { return mBaseGraph; }

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

    BaseGraph& mBaseGraph;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_TYPED_SUB_GRAPH_HPP

