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
class BaseGraph
{
public:
    virtual ~BaseGraph() {}

    typedef VertexType Vertex;
    typedef EdgeType Edge;

    /**
     * Type definitions that allow adding task properties
     */
    typedef graph_analysis::EdgeProperty<EdgeType> EdgePropertyType;
    typedef graph_analysis::VertexProperty<VertexType> VertexPropertyType;

    typedef typename EdgePropertyType::Ptr EdgePropertyTypePtr;
    typedef typename VertexPropertyType::Ptr VertexPropertyTypePtr;

    /**
     * Add a vertex
     * \return the created vertex
     */
    virtual Vertex addVertex() = 0;

    /**
     * Assign a vertex property
     */
    virtual void assignVertexProperty(const Vertex& v, VertexPropertyTypePtr property) = 0;

    /**
     * Add an edge
     * \return the created edge
     */
    virtual Edge addEdge(const Vertex& u, const Vertex& v) = 0;

    /**
     * Assign an edge property
     */
    virtual void assignEdgeProperty(const Edge& e, EdgePropertyTypePtr property) = 0;

    /**
     * Get the source vertex for this edge
     * \return Pointer to the vertex data
     */
    virtual VertexPropertyTypePtr getSourceVertex(const Edge& e) const = 0;

    /**
     * Get the target vertex for this edge
     * \return Pointer to the vertex data
     */
    virtual VertexPropertyTypePtr getTargetVertex(const Edge& e) const = 0;

    /**
     * Get access to the underlying graph
     * \return the underlying graph instance
     */
    GraphType& raw() { return mGraph; }

protected:
    GraphType mGraph;
};

/**
 * DirectedGraph based on lemon library
 */
class DirectedGraph : public BaseGraph<lemon::ListDigraph, lemon::ListDigraph::Node, lemon::ListDigraph::Arc>
{
public:
    typedef BaseGraph<lemon::ListDigraph,lemon::ListDigraph::Node, lemon::ListDigraph::Arc> GraphType;

    /**
     * Default constructor of the graph
     */
    DirectedGraph()
        : GraphType()
        , mEdgePropertyMap(mGraph)
        , mVertexPropertyMap(raw())
    {}

    typedef lemon::ListDigraph::ArcMap< GraphType::EdgePropertyTypePtr > EdgePropertyMap;
    typedef lemon::ListDigraph::NodeMap< GraphType::VertexPropertyTypePtr > VertexPropertyMap;

    typedef lemon::ListDigraph::Node VertexType;

    /**
     * Add a vertex
     * \return the created vertex
     */
    virtual Vertex addVertex() { return mGraph.addNode(); }

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
    Edge addEdge(const Vertex& u, const Vertex& v) { return mGraph.addArc(u,v); }

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
    VertexPropertyTypePtr getSourceVertex(const Edge& e) const
    {
        return mVertexPropertyMap[ mGraph.source(e) ];
    }

    /**
     * Get the target vertex for this edge
     */
    VertexPropertyTypePtr getTargetVertex(const Edge& e) const
    {
        return mVertexPropertyMap[ mGraph.target(e) ];
    }

    /**
     * Direct usage off operator= is disallowed in lemon, thus
     * need for explicit usage of copy functions
     */
    DirectedGraph& operator=(const DirectedGraph& other)
    {
        lemon::digraphCopy(other.mGraph, this->mGraph).
            nodeMap(other.mVertexPropertyMap, this->mVertexPropertyMap).
            arcMap(other.mEdgePropertyMap, this->mEdgePropertyMap);

        return *this;
    }

private:
    EdgePropertyMap mEdgePropertyMap;
    VertexPropertyMap mVertexPropertyMap;

};

}
#endif // GRAPH_ANALYSIS_GRAPH_HPP
