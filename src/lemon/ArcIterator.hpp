#ifndef GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP
#define GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP

#include "../EdgeIterator.hpp"
#include "../Vertex.hpp"

namespace graph_analysis {
namespace lemon {

template<typename T>
class ArcIterator : public EdgeIterator
{
public:
    ArcIterator(const T& graph)
        : mGraph(graph)
        , mArcIt(graph.raw())
    {}

    bool next()
    {
        while( mArcIt != ::lemon::INVALID )
        {
            Edge::Ptr edge = mGraph.mEdgeMap[mArcIt];
            ++mArcIt;
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
    typename T::graph_t::ArcIt mArcIt;

};

template<typename T>
class OutArcIterator : public EdgeIterator
{
public:
    OutArcIterator(const T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
        , mOutArcIt(graph.raw(), graph.raw().nodeFromId( vertex->getId(graph.getId())))
    {}

    bool next()
    {
        while( mOutArcIt != ::lemon::INVALID )
        {
            Edge::Ptr edge = mGraph.mEdgeMap[mOutArcIt];
            ++mOutArcIt;
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
    typename T::graph_t::OutArcIt mOutArcIt;

};


template<typename T>
class InArcIterator : public EdgeIterator
{
public:
    InArcIterator(const T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
        , mInArcIt(graph.raw(), graph.raw().nodeFromId( vertex->getId(graph.getId())))
    {}

    bool next()
    {
        while( mInArcIt != ::lemon::INVALID )
        {
            Edge::Ptr edge = mGraph.mEdgeMap[mInArcIt];
            if(skip(edge))
            {
                continue;
            }
            setNext(edge);
            ++mInArcIt;
            return true;
        }
        return false;
    }

protected:
    const T& mGraph;
    typename T::graph_t::InArcIt mInArcIt;

};


template<typename T>
class InOutArcIterator : public EdgeIterator
{
public:
    InOutArcIterator(const T& graph, Vertex::Ptr vertex)
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
    const T& mGraph;
    InArcIterator<T> mInArcIterator;
    OutArcIterator<T> mOutArcIterator;
};


} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_ARC_ITERATOR_HPP
