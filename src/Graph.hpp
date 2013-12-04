#ifndef GRAPH_ANALYSIS_GRAPH_HPP
#define GRAPH_ANALYSIS_GRAPH_HPP

#include <lemon/list_graph.h>

#include <graph_analysis/EdgeProperty.hpp>
#include <graph_analysis/VertexProperty.hpp>

namespace graph_analysis
{

/**
 * General Graph template that should be implemented to wrap specific graph library
 * functionality
 */
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
    Edge addEdge(const VertexType& u, const VertexType& v) { return Edge(); }

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
    /**
     * Default constructor of the graph
     */
    Graph()
        : mGraph(), mEdgePropertyMap(mGraph), mVertexPropertyMap(mGraph)
    {}

    typedef VertexType Vertex;
    typedef EdgeType Edge;

    /**
     * Type definitions that allow adding task properties
     */
    typedef typename graph_analysis::EdgeProperty<EdgeType> EdgePropertyType;
    typedef typename graph_analysis::VertexProperty<VertexType> VertexPropertyType;

    typedef typename EdgePropertyType::Ptr EdgePropertyTypePtr;
    typedef typename VertexPropertyType::Ptr VertexPropertyTypePtr;

    typedef lemon::ListDigraph::ArcMap< EdgePropertyTypePtr > EdgePropertyMap;
    typedef lemon::ListDigraph::NodeMap< VertexPropertyTypePtr > VertexPropertyMap;

    /**
     * Add a vertex
     * \return the created vertex
     */
    Vertex addVertex() { return mGraph.addNode(); }

    /**
     * Assign a vertex property
     */
    void assignVertexProperty(const Vertex& v, VertexPropertyTypePtr property)
    { 
        mVertexPropertyMap[v] = property;
        property->setVertex(v);
    }

    /**
     * Add an edge
     * \return the created edge
     */
    Edge addEdge(const VertexType& u, const VertexType& v) { return mGraph.addArc(u,v); }

    /**
     * Assign an edge property
     */
    void assignEdgeProperty(const Edge& e, EdgePropertyTypePtr property)
    {
        mEdgePropertyMap[e] = property;
        property->setEdge(e); 
    }

    /**
     * Get the source vertex for this edge
     */
    VertexPropertyTypePtr getSourceVertex(const Edge& e)
    {
        mVertexPropertyMap[ mGraph.source(e) ];
    }

    /**
     * Get the target vertex for this edge
     */
    VertexPropertyTypePtr getTargetVertex(const Edge& e)
    {
        mVertexPropertyMap[ mGraph.target(e) ];
    }

    /**
     * Direct usage off operator= is disallowed in lemon, thus
     * need for explicit usage of copy functions
     */
    Graph& operator=(const Graph& other)
    {
        lemon::digraphCopy(other.mGraph, this->mGraph).
            nodeMap(other.mVertexPropertyMap, this->mVertexPropertyMap).
            arcMap(other.mEdgePropertyMap, this->mEdgePropertyMap);

        return *this;
    }

    /**
     * Get raw instance of the graph, i.e. resolve to the low level library type
     */
    lemon::ListDigraph& raw() { return mGraph; }

private:
    lemon::ListDigraph mGraph;

    EdgePropertyMap mEdgePropertyMap;
    VertexPropertyMap mVertexPropertyMap;

};

typedef Graph<lemon::ListDigraph, lemon::ListDigraph::Node, lemon::ListDigraph::Arc> DirectedGraph;

}
#endif // GRAPH_ANALYSIS_GRAPH_HPP
