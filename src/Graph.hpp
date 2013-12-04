#ifndef GRAPH_ANALYSIS_GRAPH_HPP
#define GRAPH_ANALYSIS_GRAPH_HPP

#include <lemon/list_graph.h>
#include <lemon/adaptors.h>

#include <graph_analysis/EdgeProperty.hpp>
#include <graph_analysis/VertexProperty.hpp>
#include <graph_analysis/Filter.hpp>

/**
 * The main namespace of this library
 */
namespace graph_analysis
{

/**
 * \brief General Graph template that should be implemented to wrap specific graph library
 * functionality
 * \tparam GraphType The underlying graph type of the wrapped library
 * \tparam VertexType The underlying vertex type of the wrapped library
 * \tparam EdgeType The underlying edge type of the wrapped library
 *
 */
template<typename GraphType, typename VertexType, typename EdgeType>
class BaseGraph
{
public:
    /**
     * \brief Default deconstructor
     */
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
     * \brief Add a vertex
     * \return the created vertex
     */
    virtual Vertex addVertex() = 0;

    /**
     * \brief Assign a vertex property
     */
    virtual void assignVertexProperty(const Vertex& v, VertexPropertyTypePtr property) = 0;

    /**
     * \brief Add an edge
     * \return the created edge
     */
    virtual Edge addEdge(const Vertex& u, const Vertex& v) = 0;

    /**
     * \brief Assign an edge property
     */
    virtual void assignEdgeProperty(const Edge& e, EdgePropertyTypePtr property) = 0;

    /**
     * \brief Get the source vertex for this edge
     * \return Pointer to the vertex data
     */
    virtual VertexPropertyTypePtr getSourceVertex(const Edge& e) const = 0;

    /**
     * \brief Get the target vertex for this edge
     * \return Pointer to the vertex data
     */
    virtual VertexPropertyTypePtr getTargetVertex(const Edge& e) const = 0;

    /**
     * \brief Get access to the underlying graph
     * \return the underlying graph instance
     */
    GraphType& raw() { return mGraph; }

protected:
    // The underlying graph instance
    GraphType mGraph;
};

/**
 * \class DirectedGraph
 * \brief Directed graph implementation based on lemon library
 */
class DirectedGraph : public BaseGraph<lemon::ListDigraph, lemon::ListDigraph::Node, lemon::ListDigraph::Arc>
{
public:
    typedef BaseGraph<lemon::ListDigraph,lemon::ListDigraph::Node, lemon::ListDigraph::Arc> GraphType;
    typedef lemon::ListDigraph RawGraphType;

    /**
     * \brief Default constructor of the graph
     */
    DirectedGraph()
        : GraphType()
        , mEdgePropertyMap(mGraph)
        , mVertexPropertyMap(raw())
    {}

    typedef lemon::ListDigraph::ArcMap< GraphType::EdgePropertyTypePtr > EdgePropertyMap;
    typedef lemon::ListDigraph::NodeMap< GraphType::VertexPropertyTypePtr > VertexPropertyMap;

    typedef lemon::ListDigraph::Node VertexType;
    typedef lemon::SubDigraph< lemon::ListDigraph, lemon::ListDigraph::NodeMap<bool>, lemon::ListDigraph::ArcMap<bool> > SubGraph;

    /**
     * \brief Add a vertex
     * \return the created vertex
     */
    virtual Vertex addVertex() { return mGraph.addNode(); }

    /**
     * \brief Assign a vertex property
     */
    void assignVertexProperty(const Vertex& v, VertexPropertyTypePtr property)
    {
        mVertexPropertyMap[v] = property;
        property->setVertex(v);
    }

    /**
     * \brief Add an edge
     * \return the created edge
     */
    Edge addEdge(const Vertex& u, const Vertex& v) { return mGraph.addArc(u,v); }

    /**
     * \brief Assign an edge property
     */
    void assignEdgeProperty(const Edge& e, EdgePropertyTypePtr property)
    {
        mEdgePropertyMap[e] = property;
        property->setEdge(e);
    }

    /**
     * \brief Get the source vertex for this edge
     * \return Pointer to the vertex data
     */
    VertexPropertyTypePtr getSourceVertex(const Edge& e) const
    {
        return mVertexPropertyMap[ mGraph.source(e) ];
    }

    /**
     * \brief Get the target vertex for this edge
     * \return Pointer to the vertex data
     */
    VertexPropertyTypePtr getTargetVertex(const Edge& e) const
    {
        return mVertexPropertyMap[ mGraph.target(e) ];
    }

    /**
     * \brief Direct usage off operator= is disallowed in lemon, thus
     * need for explicit usage of copy functions
     */
    DirectedGraph& operator=(const DirectedGraph& other)
    {
        lemon::digraphCopy(other.mGraph, this->mGraph).
            nodeMap(other.mVertexPropertyMap, this->mVertexPropertyMap).
            arcMap(other.mEdgePropertyMap, this->mEdgePropertyMap);

        return *this;
    }

    SubGraph applyFilters(Filter<VertexPropertyTypePtr>::Ptr vertexFilter, Filter<EdgePropertyTypePtr>::Ptr edgeFilter)
    {
        lemon::ListDigraph::NodeMap<bool> nodeFilter(mGraph);
        lemon::ListDigraph::ArcMap<bool> arcFilter(mGraph);

        SubGraph subgraph(mGraph, nodeFilter, arcFilter);

        for( lemon::ListDigraph::NodeIt n(mGraph); n != lemon::INVALID; ++n)
        {
            if( vertexFilter->evaluate( mVertexPropertyMap[n] ) )
            {
                subgraph.disable(n);
            }
        }

        for( lemon::ListDigraph::ArcIt a(mGraph); a != lemon::INVALID; ++a)
        {
            if( edgeFilter->evaluate( mEdgePropertyMap[a] ) )
            {
                subgraph.disable(a);
            }
        }

        return subgraph;
    }


private:
    // Property maps to store data associated with vertices and edges
    EdgePropertyMap mEdgePropertyMap;
    VertexPropertyMap mVertexPropertyMap;
};

}
#endif // GRAPH_ANALYSIS_GRAPH_HPP
