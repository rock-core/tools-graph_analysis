#ifndef GRAPH_ANALYSIS_BOOST_GRAPH_NODE_ITERATOR_HPP
#define GRAPH_ANALYSIS_BOOST_GRAPH_NODE_ITERATOR_HPP

#include <graph_analysis/VertexIterator.hpp>

namespace graph_analysis {
namespace boost_graph {

template<typename T>
class NodeIterator : public VertexIterator
{
public:
    NodeIterator(const T& graph)
        : mGraph(graph)
    {
        boost::tie(mStart, mEnd) = boost::vertices(mGraph.raw());
        mCurrent = mStart;
    }

    bool next()
    {
        while(mCurrent != mEnd)
        {
            VertexDescriptor vertexDescriptor = *mCurrent;
            Vertex::Ptr vertex = mGraph.raw()[vertexDescriptor];
            ++mCurrent;
            if(skip(vertex))
            {
                continue;
            }
            setNext(vertex);
            return true;
        }
        return false;
    }
private:
    const T& mGraph;

    VertexIteratorImpl mCurrent;
    VertexIteratorImpl mStart;
    VertexIteratorImpl mEnd;
};

} // end namespace boost_graph
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_BOOST_GRAPH_NODE_ITERATOR_HPP
