#ifndef GRAPH_ANALYSIS_SNAP_GRAPH_HPP
#define GRAPH_ANALYSIS_SNAP_GRAPH_HPP

#include <map>
#include <snap/snap-core/Snap.h>
#include <graph_analysis/Graph.hpp>

namespace graph_analysis {
namespace snap {

/**
 * \class DirectedGraph
 * \brief Directed graph implementation based on lemon library
 */
class DirectedGraph : public BaseGraph< PNEGraph, TInt, TInt>
{
public:
    typedef BaseGraph< PNEGraph, TInt, TInt> GraphType;
    typedef PNEGraph RawGraphType;
    typedef PNEGraph SubGraph;

    typedef std::map<TInt, EdgePropertyTypePtr> EdgePropertyMap;
    typedef std::map<TInt, VertexPropertyTypePtr> VertexPropertyMap;
    /**
     * \brief Default constructor of the graph
     */
    DirectedGraph()
        : GraphType()
    {
        mGraph = TNEGraph::New();
    }

    /**
     * \brief Add a vertex
     * \return the created vertex
     */
    virtual Vertex addVertex() { return mGraph->AddNode(); }

    /**
     * \brief Assign a vertex property
     */
    void assignVertexProperty(const Vertex& v, VertexPropertyTypePtr property)
    {
        mVertexPropertyMap[v] = property;
        property->addVertex(this, v);
    }

    /**
     * \brief Add an edge
     * \return an edge interator
     */
    Edge addEdge(const Vertex& u, const Vertex& v) 
    { 
        return mGraph->AddEdge(u,v); 
    }

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
        TInt nodeId = mGraph->GetEI(e).GetSrcNId();

        VertexPropertyMap::const_iterator cit = mVertexPropertyMap.find(nodeId);
        if(cit != mVertexPropertyMap.end())
        {
            return cit->second;
        }

        throw std::runtime_error("SNAP: could not find source vertex for edge");
    }

    /**
     * \brief Get the target vertex for this edge
     * \return Pointer to the vertex data
     */
    VertexPropertyTypePtr getTargetVertex(const Edge& e) const
    {
        TInt nodeId =  mGraph->GetEI(e).GetDstNId();

        VertexPropertyMap::const_iterator cit = mVertexPropertyMap.find(nodeId);
        if(cit != mVertexPropertyMap.end())
        {
            return cit->second;
        }

        throw std::runtime_error("SNAP: could not find target vertex for edge");
    }

    SubGraph applyFilters(Filter<VertexPropertyTypePtr>::Ptr vertexFilter, Filter<EdgePropertyTypePtr>::Ptr edgeFilter)
    {
        SubGraph subgraph;
        TSnap::GetSubGraph(mGraph, TIntV::GetV(0));

        return subgraph;
    }
private:
    EdgePropertyMap mEdgePropertyMap;
    VertexPropertyMap mVertexPropertyMap;
};

} // end namespace snap
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_SNAP_GRAPH_HPP
