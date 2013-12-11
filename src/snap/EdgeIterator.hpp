#ifndef GRAPH_ANALYSIS_SNAP_EDGE_ITERATOR_HPP
#define GRAPH_ANALYSIS_SNAP_EDGE_ITERATOR_HPP

#include <graph_analysis/VertexIterator.hpp>

namespace graph_analysis {
namespace snap {

template<typename T>
class EdgeIterator : public graph_analysis::EdgeIterator
{
public:
    EdgeIterator(T& graph)
        : mGraph(graph)
        , mEdgeIt(graph.raw()->BegEI())
    {}

    bool next()
    {
        if( mEdgeIt != mGraph.raw()->EndEI() )
        {
            setNext( mGraph.mEdgeMap[mEdgeIt.GetId()] );
            mEdgeIt++;
            return true;
        }
        return false;
    }

protected:
    T& mGraph;
    typename TNEGraph::TEdgeI mEdgeIt;

};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_EDGE_ITERATOR_HPP
