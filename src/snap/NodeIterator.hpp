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
        , mNodeIt(graph.raw().BegNI())
    {}

    bool next()
    {
        while( mNodeIt != mGraph.raw().EndNI() )
        {
            Vertex::Ptr vertex = mGraph.getVertex(mNodeIt.GetId());
            mNodeIt++;
            if(skip(vertex))
            {
                continue;
            }
            setNext(vertex);
            return true;
        }
        return false;
    }

protected:
    T& mGraph;
    typename T::graph_t::TNodeI mNodeIt;

};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_NODE_ITERATOR_HPP
