#ifndef GRAPH_ANALYSIS_SUB_GRAPH_IMPL_HPP
#define GRAPH_ANALYSIS_SUB_GRAPH_IMPL_HPP

#include <assert.h>
#include <graph_analysis/SubGraph.hpp>

namespace graph_analysis {

/**
 * \brief SubGraphImpl represents the subgraph implemention
 * \details This wrapper guarantees that vertex filters and edgefilters won't be deallocated
 * as long as the subgraph is in use, e.g., for the lemon library the subgraph holds a reference to the main graph and only
 * associates filters -- but since the filters are added as references they need to be managed properly.
 * \tparam SubGraphType the underlying subgraph type
 * \tparam VertexFilter the vertex filter
 * \tparam EdgeFilter the edge filter
 */
template<typename SubGraphType, typename VertexFilter, typename EdgeFilter>
class SubGraphImpl : public SubGraph
{
public:
    typedef SubGraphType graph_t;
    typedef SubGraphImpl<SubGraphType, VertexFilter, EdgeFilter> BaseSubGraphType;

    typedef VertexFilter vertex_filter_t;
    typedef EdgeFilter edge_filter_t;

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
    SubGraphImpl(BaseGraph* graph, VertexFilter* vertexFilter, EdgeFilter* edgeFilter)
        : SubGraph(graph)
    {
        mpVertexFilter = boost::shared_ptr<VertexFilter>(vertexFilter);
        mpEdgeFilter = boost::shared_ptr<EdgeFilter>(edgeFilter);
    }

    /**
     * Retrieve the raw subgraph type. Use with care since this object's memory might
     * be deallocated if you release your main object
     */
    graph_t& raw()
    {
        assert(mpSubgraph);
        return (*mpSubgraph);
    }

    const graph_t& raw() const
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
    void setSubgraph(graph_t* subgraph)
    {
        assert(subgraph);
        mpSubgraph = boost::shared_ptr<graph_t>(subgraph);
    }

    boost::shared_ptr<VertexFilter> mpVertexFilter;
    boost::shared_ptr<EdgeFilter> mpEdgeFilter;

    boost::shared_ptr<graph_t> mpSubgraph;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SUB_GRAPH_IMPL_HPP

