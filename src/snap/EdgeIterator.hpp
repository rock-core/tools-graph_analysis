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
    T& mGraph;
    typename T::graph_t::TEdgeI mEdgeIt;

};

template<typename T>
class OutEdgeIterator : public graph_analysis::EdgeIterator
{
public:
    OutEdgeIterator(T& graph, typename T::graph_t::TNodeI nodeIterator)
        : mGraph(graph)
        , mCurrentIndex(0)
        , mNodeIt(nodeIterator)
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
    T& mGraph;
    int mCurrentIndex;
    typename T::graph_t::TNodeI mNodeIt;
};

template<typename T>
class InEdgeIterator : public graph_analysis::EdgeIterator
{
public:
    InEdgeIterator(T& graph, typename T::graph_t::TNodeI nodeIterator)
        : mGraph(graph)
        , mCurrentIndex(0)
        , mNodeIt(nodeIterator)
    {}

    bool next()
    {
        while( mCurrentIndex != mNodeIt.GetInDeg())
        {
            typename T::graph_t::TEdgeI edgeIt = mGraph.raw().GetEI(mNodeIt.GetId(), mNodeIt.GetInNId(mCurrentIndex));
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
    T& mGraph;
    int mCurrentIndex;
    typename T::graph_t::TNodeI mNodeIt;
};
} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_EDGE_ITERATOR_HPP
