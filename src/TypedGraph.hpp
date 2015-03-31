#ifndef GRAPH_ANALYSIS_TYPED_GRAPH_HPP
#define GRAPH_ANALYSIS_TYPED_GRAPH_HPP

namespace graph_analysis {

template<typename T>
class TypedGraph : public BaseGraph
{
public:
    TypedGraph()
        : mGraph()
    {}

    typedef T graph_t;

    /**
     * Return underlying raw graph instance
     */
    graph_t& raw() { return mGraph; }

protected:
    graph_t mGraph;

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_TYPED_GRAPH_HPP
