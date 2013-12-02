#ifndef GRAPH_ANALYSIS_GRAPH_HPP
#define GRAPH_ANALYSIS_GRAPH_HPP

#include <lemon/list_graph.h>
#include <boost/shared_ptr.hpp>

namespace graph_analysis
{

template<typename GraphType, typename VertexType, typename EdgeType>
class Graph
{
public:
    typedef VertexType Vertex;
    typedef EdgeType Edge;

    /**
     * Add a vertex
     * \return the created vertex
     */
    Vertex addVertex() { return Vertex(); }

    /**
     * Add an edge
     * \return the created edge
     */
    Edge addEdge(VertexType u, VertexType v) { return Edge(); }

    /**
     * Get access to the underlying graph
     */
    GraphType& raw() { return mGraph; }

private:
    GraphType mGraph;

};

/**
 * Template specialization for lemon's directed graph
 */
template<typename VertexType, typename EdgeType>
class Graph<lemon::ListDigraph, VertexType, EdgeType>
{
public:
    typedef VertexType Vertex;
    typedef EdgeType Edge;

    /**
     * Add a vertex
     * \return the created vertex
     */
    Vertex addVertex() { return mGraph.addNode(); }

    /**
     * Add an edge
     * \return the created edge
     */
    Edge addEdge(VertexType u, VertexType v) { return mGraph.addArc(u,v); }


    /**
     * Direct usage off operator= is disallowed in lemon, thus
     * need for explicit usage of copy functions
     */
    Graph& operator=(const Graph& other)
    {
        lemon::digraphCopy(other.mGraph, this->mGraph);
        return *this;
    }

    lemon::ListDigraph& raw() { return mGraph; }

private:
    lemon::ListDigraph mGraph;
};

typedef Graph<lemon::ListDigraph, lemon::ListDigraph::Node, lemon::ListDigraph::Arc> DirectedGraph;

}
#endif // GRAPH_ANALYSIS_GRAPH_HPP
