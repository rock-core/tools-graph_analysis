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

    typedef T GraphType;

    /**
     * Return underlying raw graph instance
     */
    GraphType& raw() { return mGraph; }

protected:
    GraphType mGraph;

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_TYPED_GRAPH_HPP
