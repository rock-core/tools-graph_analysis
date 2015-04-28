#ifndef GRAPH_ANALYSIS_BOOST_GRAPH_ARC_ITERATOR_HPP
#define GRAPH_ANALYSIS_BOOST_GRAPH_ARC_ITERATOR_HPP

#include <graph_analysis/EdgeIterator.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace boost_graph {

template<typename T>
class ArcIterator : public EdgeIterator
{
    typedef typename boost::graph_traits< typename T::graph_t >::edge_iterator EdgeIteratorImpl;
    typedef typename boost::graph_traits< typename T::graph_t >::edge_descriptor EdgeDescriptor;

public:
    ArcIterator(const T& graph)
        : mGraph(graph)
    {
        boost::tie(mStart, mEnd) = boost::edges(mGraph.raw());
        mCurrent = mStart;
    }

    bool next()
    {
        while( mCurrent != mEnd)
        {
            EdgeDescriptor edgeDescriptor = *mCurrent;
            Edge::Ptr edge = mGraph.raw()[edgeDescriptor];
            ++mCurrent;
            if( skip(edge))
            {
                continue;
            }
            setNext(edge);
            return true;
        }
        return false;
    }

private:
    EdgeIteratorImpl mCurrent;
    EdgeIteratorImpl mStart;
    EdgeIteratorImpl mEnd;

    const T& mGraph;
};

template<typename T>
class OutArcIterator : public EdgeIterator
{
    typedef typename boost::graph_traits< typename T::graph_t >::out_edge_iterator EdgeIteratorImpl;
    typedef typename boost::graph_traits< typename T::graph_t >::edge_descriptor EdgeDescriptor;
    typedef typename boost::graph_traits< typename T::graph_t >::vertex_descriptor VertexDescriptor;

public:
    OutArcIterator(const T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
    {
        GraphElementId id = mGraph.getVertexId(vertex);
        typename T::VertexMap::const_iterator cit = mGraph.mVertexMap.find(id);
        if(cit == mGraph.mVertexMap.end())
        {
            throw std::invalid_argument("graph_analysis::boost_graph::OutArcIterator: '" + vertex->toString() + "' is not part of this graph");
        }
        VertexDescriptor vertexDescriptor = cit->second;
        boost::tie(mStart, mEnd) = boost::out_edges(vertexDescriptor, mGraph.raw());
        mCurrent = mStart;
    }

    bool next()
    {
        while( mCurrent != mEnd)
        {
            EdgeDescriptor edgeDescriptor = *mCurrent;
            Edge::Ptr edge = mGraph.raw()[edgeDescriptor];
            ++mCurrent;
            if( skip(edge))
            {
                continue;
            }
            setNext(edge);
            return true;
        }
        return false;
    }

private:
    EdgeIteratorImpl mCurrent;
    EdgeIteratorImpl mStart;
    EdgeIteratorImpl mEnd;

    const T& mGraph;
};

template<typename T>
class InArcIterator : public EdgeIterator
{
    typedef typename boost::graph_traits< typename T::graph_t >::in_edge_iterator EdgeIteratorImpl;
    typedef typename boost::graph_traits< typename T::graph_t >::edge_descriptor EdgeDescriptor;
    typedef typename boost::graph_traits< typename T::graph_t >::vertex_descriptor VertexDescriptor;

public:
    InArcIterator(const T& graph, Vertex::Ptr vertex)
        : mGraph(graph)
    {
        GraphElementId id = mGraph.getVertexId(vertex);
        typename T::VertexMap::const_iterator cit = mGraph.mVertexMap.find(id);
        if(cit == mGraph.mVertexMap.end())
        {
            throw std::invalid_argument("graph_analysis::boost_graph::InArcIterator: '" + vertex->toString() + "' is not part of this graph");
        }
        VertexDescriptor vertexDescriptor = cit->second;
        boost::tie(mStart, mEnd) = boost::in_edges(vertexDescriptor, mGraph.raw());
        mCurrent = mStart;
    }

    bool next()
    {
        while( mCurrent != mEnd)
        {
            EdgeDescriptor edgeDescriptor = *mCurrent;
            Edge::Ptr edge = mGraph.raw()[edgeDescriptor];
            ++mCurrent;
            if( skip(edge))
            {
                continue;
            }
            setNext(edge);
            return true;
        }
        return false;
    }

private:
    EdgeIteratorImpl mCurrent;
    EdgeIteratorImpl mStart;
    EdgeIteratorImpl mEnd;

    const T& mGraph;
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

} // end namespace boost_graph
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_BOOST_GRAPH_ARC_ITERATOR_HPP
