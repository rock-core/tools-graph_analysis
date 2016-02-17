#ifndef GRAPH_ANALYSIS_SNAP_EDGE_ITERATOR_HPP
#define GRAPH_ANALYSIS_SNAP_EDGE_ITERATOR_HPP

#include <graph_analysis/VertexIterator.hpp>

namespace graph_analysis {
namespace snap {

template<typename T>
class EdgeIterator : public graph_analysis::EdgeIterator
{
public:
    EdgeIterator(const T& graph)
        : mGraph(graph)
        , mEdgeIt(graph.raw().BegEI())
    {}

    bool next()
    {
        while( mEdgeIt != mGraph.raw().EndEI() )
        {
            Edge::Ptr edge = mGraph.getEdge(mEdgeIt.GetId());
            mEdgeIt++;
            if(skip(edge))
            {
                continue;
            }
            setNext(edge);
            return true;
        }
        return false;
    }

protected:
    const T& mGraph;
    typename T::graph_t::TEdgeI mEdgeIt;

};

template<typename T>
class OutEdgeIterator : public graph_analysis::EdgeIterator
{
public:
    OutEdgeIterator(const T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
        , mCurrentIndex(0)
        , mNodeIt(mGraph.raw().GetNI( mGraph.getVertexId(vertex)) )
    {}

    bool next()
    {
        while( mCurrentIndex != mNodeIt.GetOutDeg())
        {
            typename T::graph_t::TEdgeI edgeIt = mGraph.raw().GetEI(mNodeIt.GetId(), mNodeIt.GetOutNId(mCurrentIndex));
            Edge::Ptr edge = mGraph.getEdge(edgeIt.GetId());
            ++mCurrentIndex;
            if(skip(edge))
            {
                continue;
            }
            setNext(edge);
            return true;
        }
        return false;
    }

protected:
    const T& mGraph;
    int mCurrentIndex;
    typename T::graph_t::TNodeI mNodeIt;
};

template<typename T>
class InEdgeIterator : public graph_analysis::EdgeIterator
{
public:
    InEdgeIterator(const T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
        , mCurrentIndex(0)
        , mNodeIt(mGraph.raw().GetNI( mGraph.getVertexId(vertex)) )
    {}

    bool next()
    {
        while( mCurrentIndex != mNodeIt.GetInDeg())
        {
            typename T::graph_t::TEdgeI edgeIt = mGraph.raw().GetEI(mNodeIt.GetInNId(mCurrentIndex), mNodeIt.GetId());
            Edge::Ptr edge = mGraph.getEdge(edgeIt.GetId());
            ++mCurrentIndex;
            if(skip(edge))
            {
                continue;
            }
            setNext( edge );
            return true;
        }
        return false;
    }

protected:
    const T& mGraph;
    int mCurrentIndex;
    typename T::graph_t::TNodeI mNodeIt;
};

template<typename T>
class InOutEdgeIterator : public graph_analysis::EdgeIterator
{
public:
    InOutEdgeIterator(const T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
        , mInEdgeIterator(graph, vertex)
        , mOutEdgeIterator(graph, vertex)
    {}

    bool next()
    {
        if(mInEdgeIterator.next())
        {
            setNext( mInEdgeIterator.current() );
            return true;
        }

        if(mOutEdgeIterator.next())
        {
            setNext( mOutEdgeIterator.current() );
            return true;
        }

        return false;
    }


protected:
    const T& mGraph;
    InEdgeIterator<T> mInEdgeIterator;
    OutEdgeIterator<T> mOutEdgeIterator;
};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_EDGE_ITERATOR_HPP
