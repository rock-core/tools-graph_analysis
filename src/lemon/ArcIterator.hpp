#ifndef GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP
#define GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP

#include <graph_analysis/EdgeIterator.hpp>

namespace graph_analysis {
namespace lemon {

template<typename T>
class ArcIterator : public EdgeIterator
{
public:
    ArcIterator(T& graph)
        : mGraph(graph)
        , mArcIt(graph.raw())
    {}

    bool next()
    {
        if( mArcIt != ::lemon::INVALID )
        {
            setNext( mGraph.mEdgeMap[mArcIt] );
            ++mArcIt;
            return true;
        }
        return false;
    }

protected:
    T& mGraph;
    typename T::GraphType::ArcIt mArcIt;

};

} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP
