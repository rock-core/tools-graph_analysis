#ifndef GRAPH_ANALYSIS_TYPED_GRAPH_HPP
#define GRAPH_ANALYSIS_TYPED_GRAPH_HPP

#include "BaseGraph.hpp"

namespace graph_analysis {

template<typename T, typename G = BaseGraph>
class TypedGraph : public G
{
public:
    TypedGraph(BaseGraph::ImplementationType type, bool directed = true)
        : G(type, directed)
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
