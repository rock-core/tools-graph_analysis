#ifndef GRAPH_ANALYSIS_SUB_GRAPH_IMPL_HPP
#define GRAPH_ANALYSIS_SUB_GRAPH_IMPL_HPP

#include <assert.h>
#include "SubGraph.hpp"
#include "BaseGraph.hpp"

namespace graph_analysis {

/**
 * \brief SubGraphImpl represents the subgraph implemention
 * \tparam SubGraphType the underlying subgraph type
 */
template<typename SubGraphType>
class SubGraphImpl : public SubGraph
{
public:
    typedef SubGraphType graph_t;

    /**
     * This default Constructor will not(!) initialize the underlying subgraph type, but
     * only the given filters.
     * Make sure that any class inheriting from this one properly initializes the subgraph, i.e.
     * using setSubgraph
     *
     * Ownership of all objects, i.e. filters and graph is transferred to this class, which internally
     * uses shared pointer to guarantee cleanup when necessary
     *
     * Classes that inherit from TypedSubGraph need to explicitly call
     * setSubgraph to link to the underlying SubGraph implementation
     */
    SubGraphImpl(shared_ptr<BaseGraph> graph)
        : SubGraph(graph)
    {}

    virtual ~SubGraphImpl() {}

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
     * Set the subgraph
     * Method to allow application of implementation specific subgraph constructors
     * \param subgraph pointer to subgraph
     */
    void setSubgraph(graph_t* subgraph)
    {
        assert(subgraph);
        mpSubgraph = shared_ptr<graph_t>(subgraph);
    }

    shared_ptr<graph_t> mpSubgraph;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SUB_GRAPH_IMPL_HPP

