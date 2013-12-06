#ifndef GRAPH_ANALYSIS_LEMON_GRAPH_HPP
#define GRAPH_ANALYSIS_LEMON_GRAPH_HPP

#include <lemon/list_graph.h>
#include <lemon/adaptors.h>

#include <graph_analysis/Graph.hpp>
#include <base/logging.h>

namespace graph_analysis {
namespace lemon {

class DirectedSubGraph : public BaseSubGraph< ::lemon::SubDigraph< ::lemon::ListDigraph, ::lemon::ListDigraph::NodeMap<bool>, ::lemon::ListDigraph::ArcMap<bool> >, ::lemon::ListDigraph::NodeMap<bool>, ::lemon::ListDigraph::ArcMap<bool> >
{
public:
    DirectedSubGraph(::lemon::ListDigraph& graph)
        : BaseSubGraphType(new VertexFilterType(graph), new EdgeFilterType(graph))
    {
         setSubgraph( new GraphType(graph, getVertexFilter(), getEdgeFilter()) );
    }
};

/**
 * \class DirectedGraph
 * \brief Directed graph implementation based on lemon library
 */
class DirectedGraph : public BaseGraph< ::lemon::ListDigraph, ::lemon::ListDigraph::Node, ::lemon::ListDigraph::Arc>
{
public:
    typedef BaseGraphType::RawGraphType RawGraphType;
    typedef DirectedSubGraph SubGraph;

    /**
     * \brief Default constructor of the graph
     */
    DirectedGraph()
        : BaseGraphType()
        , mEdgePropertyMap(mGraph)
        , mVertexPropertyMap(raw())
    {}

    typedef BaseGraphType::EdgePropertyType EdgeProperty;
    typedef BaseGraphType::VertexPropertyType VertexProperty;

    typedef ::lemon::ListDigraph::ArcMap< BaseGraphType::EdgePropertyTypePtr > EdgePropertyMap;
    typedef ::lemon::ListDigraph::NodeMap< BaseGraphType::VertexPropertyTypePtr > VertexPropertyMap;

    typedef ::lemon::ListDigraph::Node VertexType;

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
        property->addVertex(this,v);
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
        property->addEdge(this, e);
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
        ::lemon::digraphCopy(other.mGraph, this->mGraph).
            nodeMap(other.mVertexPropertyMap, this->mVertexPropertyMap).
            arcMap(other.mEdgePropertyMap, this->mEdgePropertyMap);

        return *this;
    }

    DirectedSubGraph applyFilters(Filter<VertexPropertyTypePtr>::Ptr vertexFilter, Filter<EdgePropertyTypePtr>::Ptr edgeFilter)
    {
        DirectedSubGraph subgraph(mGraph);

        if(vertexFilter)
        {
            for( ::lemon::ListDigraph::NodeIt n(mGraph); n != ::lemon::INVALID; ++n)
            {
                if( vertexFilter->evaluate( mVertexPropertyMap[n] ) )
                {
                    subgraph.raw().disable(n);
                } else {
                    subgraph.raw().enable(n);
                }
            }
        }

        if(edgeFilter)
        {
            for( ::lemon::ListDigraph::ArcIt a(mGraph); a != ::lemon::INVALID; ++a)
            {
                if( edgeFilter->evaluate( mEdgePropertyMap[a] ) )
                {
                    subgraph.raw().disable(a);
                } else {
                    subgraph.raw().enable(a);
                }
            }
        }

        return subgraph;
    }


private:
    // Property maps to store data associated with vertices and edges
    EdgePropertyMap mEdgePropertyMap;
    VertexPropertyMap mVertexPropertyMap;
};


} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_GRAPH_HPP
