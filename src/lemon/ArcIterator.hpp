#ifndef GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP
#define GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP

#include <graph_analysis/EdgeIterator.hpp>
#include <graph_analysis/Vertex.hpp>

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

template<typename T>
class OutArcIterator : public EdgeIterator
{
public:
    OutArcIterator(T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
        , mOutArcIt(graph.raw(), graph.raw().nodeFromId( vertex->getId(graph.getId())))
    {}

    bool next()
    {
        if( mOutArcIt != ::lemon::INVALID )
        {
            setNext( mGraph.mEdgeMap[mOutArcIt] );
            ++mOutArcIt;
            return true;
        }
        return false;
    }

protected:
    T& mGraph;
    typename T::GraphType::OutArcIt mOutArcIt;

};


template<typename T>
class InArcIterator : public EdgeIterator
{
public:
    InArcIterator(T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
        , mInArcIt(graph.raw(), graph.raw().nodeFromId( vertex->getId(graph.getId())))
    {}

    bool next()
    {
        if( mInArcIt != ::lemon::INVALID )
        {
            setNext( mGraph.mEdgeMap[mInArcIt] );
            ++mInArcIt;
            return true;
        }
        return false;
    }

protected:
    T& mGraph;
    typename T::GraphType::InArcIt mInArcIt;

};


template<typename T>
class InOutArcIterator : public EdgeIterator
{
public:
    InOutArcIterator(T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
        , mInArcIterator(graph, vertex)
        , mOutArcIterator(graph, vertex)
    {}

    bool next()
    {
        if(mInArcIterator.next())
        {
            setNext( mInArcIterator.current() );
            return true;
        }

        if(mOutArcIterator.next())
        {
            setNext( mOutArcIterator.current() );
            return true;
        }

        return false;
    }


protected:
    T& mGraph;
    InArcIterator<T> mInArcIterator;
    OutArcIterator<T> mOutArcIterator;
};


} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP
