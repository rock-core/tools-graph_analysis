#ifndef GRAPH_ANALYSIS_TYPED_GRAPH_HPP
#define GRAPH_ANALYSIS_TYPED_GRAPH_HPP

#include <graph_analysis/BaseGraph.hpp>

namespace graph_analysis {

template<typename T>
class TypedGraph : public BaseGraph
{
public:
    TypedGraph(ImplementationType type)
        : BaseGraph(type)
    {}

    virtual ~TypedGraph() {};

    typedef T graph_t;

    /**
     * Return underlying raw graph instance
     */
    graph_t& raw() { return mGraph; }

    const graph_t& raw() const { return mGraph; }

protected:
    graph_t mGraph;

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_TYPED_GRAPH_HPP
