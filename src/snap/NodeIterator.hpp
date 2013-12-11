#ifndef GRAPH_ANALYSIS_SNAP_NODE_ITERATOR_HPP
#define GRAPH_ANALYSIS_SNAP_NODE_ITERATOR_HPP

#include <graph_analysis/VertexIterator.hpp>

namespace graph_analysis {
namespace snap {

template<typename T>
class NodeIterator : public VertexIterator
{
public:
    NodeIterator(T& graph)
        : mGraph(graph)
        , mNodeIt(graph.raw()->BegNI())
    {}

    bool next()
    {
        if( mNodeIt != mGraph.raw()->EndNI() )
        {
            setNext( mGraph.mVertexMap[mNodeIt.GetId()] );
            mNodeIt++;
            return true;
        }
        return false;
    }

protected:
    T& mGraph;
    typename TNEGraph::TNodeI mNodeIt;

};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_NODE_ITERATOR_HPP
