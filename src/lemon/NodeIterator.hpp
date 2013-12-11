#ifndef GRAPH_ANALYSIS_LEMON_NODE_ITERATOR_HPP
#define GRAPH_ANALYSIS_LEMON_NODE_ITERATOR_HPP

#include <graph_analysis/VertexIterator.hpp>

namespace graph_analysis {
namespace lemon {

template<typename T>
class NodeIterator : public VertexIterator
{
public:
    NodeIterator(T& graph)
        : mGraph(graph)
        , mNodeIt(graph.raw())
    {}

    bool next()
    {
        if( mNodeIt != ::lemon::INVALID )
        {
            setNext( mGraph.mVertexMap[mNodeIt] );
            ++mNodeIt;
            return true;
        }
        return false;
    }

protected:
    T& mGraph;
    typename T::GraphType::NodeIt mNodeIt;

};

} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_NODE_ITERATOR_HPP
