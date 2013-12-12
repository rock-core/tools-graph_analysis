#ifndef GRAPH_ANALYSIS_LEMON_GRAPH_HPP
#define GRAPH_ANALYSIS_LEMON_GRAPH_HPP

#include <lemon/list_graph.h>
#include <lemon/adaptors.h>
#include <lemon/lgf_writer.h>

#include <graph_analysis/Graph.hpp>
#include <graph_analysis/lemon/NodeIterator.hpp>
#include <graph_analysis/lemon/ArcIterator.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace lemon {

typedef ::lemon::SubDigraph< ::lemon::ListDigraph, ::lemon::ListDigraph::NodeMap<bool>, ::lemon::ListDigraph::ArcMap<bool> > SubGraph;

class DirectedSubGraph : public TypedSubGraph< SubGraph, ::lemon::ListDigraph::NodeMap<bool>, ::lemon::ListDigraph::ArcMap<bool> >
{
public:
    DirectedSubGraph(::lemon::ListDigraph& graph)
        : TypedSubGraph(new VertexFilterType(graph), new EdgeFilterType(graph))
    {
         setSubgraph( new GraphType(graph, getVertexFilter(), getEdgeFilter()) );
    }
};



/**
 * \class DirectedGraph
 * \brief Directed graph implementation based on lemon library
 */
class DirectedGraph : public TypedGraph< ::lemon::ListDigraph >
{
public:
    /**
     * \brief Default constructor of the graph
     */
    DirectedGraph()
        : TypedGraph()
        , mEdgeMap(raw())
        , mVertexMap(raw())
    {}

    typedef GraphType::ArcMap< Edge::Ptr > EdgeMap;
    typedef GraphType::NodeMap< Vertex::Ptr > VertexMap;

    typedef GraphType::NodeMap<bool> VertexActivationMap;
    typedef GraphType::ArcMap<bool> EdgeActivationMap;

    // Register the DirecteSubGraph as official SubGraph
    typedef DirectedSubGraph SubGraph;

    friend class NodeIterator<DirectedGraph>;
    friend class ArcIterator<DirectedGraph>;

    /**
     * \brief Add a vertex
     * \return the created vertex
     */
    virtual GraphElementId addVertex(Vertex::Ptr vertex)
    {
        BaseGraph::addVertex(vertex);

        GraphType::Node node = mGraph.addNode();
        int nodeId = mGraph.id(node);
        mVertexMap[node] = vertex;

        vertex->associate(getId(), nodeId);
        return nodeId;
    }

    /**
     * \brief Add an edge
     * \return the created edge
     */
    GraphElementId addEdgeInternal(Edge::Ptr edge, GraphElementId sourceVertexId, GraphElementId targetVertexId)
    {
        GraphType::Node sourceNode = mGraph.nodeFromId( sourceVertexId );
        GraphType::Node targetNode = mGraph.nodeFromId( targetVertexId );

        GraphType::Arc arc = mGraph.addArc(sourceNode, targetNode);
        int arcId = mGraph.id(arc);
        edge->associate(getId(), arcId);
        mEdgeMap[arc] = edge;

        return arcId;
    }
    }

    /**
     * \brief Get the source vertex for this edge
     * \return Pointer to the vertex data
     */
    Vertex::Ptr getSourceVertex(Edge::Ptr e) const
    {
        GraphElementId edgeId = getEdgeId(e);
        return mVertexMap[ mGraph.source( mGraph.arcFromId(edgeId)) ];
    }

    /**
     * \brief Get the target vertex for this edge
     * \return Pointer to the vertex data
     */
    Vertex::Ptr getTargetVertex(const Edge::Ptr& e) const
    {
        GraphElementId edgeId = getEdgeId(e);
        return mVertexMap[ mGraph.target( mGraph.arcFromId(edgeId)) ];
    }

    void removeVertex(Vertex::Ptr vertex)
    {
        BaseGraph::removeVertex(vertex);

        int nodeId = getVertexId( vertex );
        GraphType::Node node = mGraph.nodeFromId(nodeId);
        mGraph.erase(node);
    }

    void removeEdge(Edge::Ptr edge)
    {
        BaseGraph::removeEdge(edge);

        int edgeId = getEdgeId(edge);
        GraphType::Arc arc = mGraph.arcFromId(edgeId);
        mGraph.erase(arc);
    }

    /**
     * \brief Direct usage off operator= is disallowed in lemon, thus
     * need for explicit usage of copy functions
     */
    DirectedGraph& operator=(const DirectedGraph& other)
    {
        ::lemon::digraphCopy(other.mGraph, this->mGraph).
            nodeMap(other.mVertexMap, this->mVertexMap).
            arcMap(other.mEdgeMap, this->mEdgeMap);

        return *this;
    }

    DirectedSubGraph applyFilters(Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter)
    {
        DirectedSubGraph subgraph(mGraph);

        if(vertexFilter)
        {
            for( GraphType::NodeIt n(mGraph); n != ::lemon::INVALID; ++n)
            {
                if( vertexFilter->evaluate( mVertexMap[n] ) )
                {
                    subgraph.raw().disable(n);
                } else {
                    subgraph.raw().enable(n);
                }
            }
        }

        if(edgeFilter)
        {
            for( GraphType::ArcIt a(mGraph); a != ::lemon::INVALID; ++a)
            {
                if( edgeFilter->evaluate( mEdgeMap[a] ) )
                {
                    subgraph.raw().disable(a);
                } else {
                    subgraph.raw().enable(a);
                }
            }
        }

        return subgraph;
    }

    void write(std::ostream& ostream = std::cout) const
    {
        // Workaround:
        // operator<
        // will be overloaded ambiguously due to using shared_ptr
        // Use explicit conversion to string map first
        typedef GraphType::ArcMap< std::string > EdgeStringMap;
        typedef GraphType::NodeMap< std::string > VertexStringMap;

        typedef GraphType::ArcMap< GraphElementId > EdgeIdMap;
        typedef GraphType::NodeMap< GraphElementId > VertexIdMap;


        EdgeStringMap edgeStringMap(mGraph);
        VertexStringMap vertexStringMap(mGraph);
        EdgeIdMap edgeIdMap(mGraph);
        VertexIdMap vertexIdMap(mGraph);

        for(GraphType::ArcIt a(mGraph); a != ::lemon::INVALID; ++a)
        {
            Edge::Ptr edge = mEdgeMap[a];
            if(edge)
            {
                edgeStringMap[a] = edge->toString();
                edgeIdMap[a] = getEdgeId(edge);
            }
        }

        for(GraphType::NodeIt n(mGraph); n != ::lemon::INVALID; ++n)
        {
            Vertex::Ptr vertex = mVertexMap[n];
            if(vertex)
            {
                vertexStringMap[n] = vertex->toString();
                vertexIdMap[n] = getVertexId(vertex); 
            }
        }

        ::lemon::digraphWriter(mGraph, ostream).
            arcMap("edges", edgeStringMap).
            nodeMap("vertices", vertexStringMap).
            arcMap("edgeId", edgeIdMap).
            nodeMap("vertexId", vertexIdMap).
            attribute("caption", "test").
            run();
    }

    /**
     * Get the vertex iterator for this implementation
     */
    VertexIterator::Ptr getVertexIterator()
    {
        NodeIterator<DirectedGraph>* it = new NodeIterator<DirectedGraph>(*this);
        return VertexIterator::Ptr(it);
    }

    /**
     * Get the edge iterator for this implementation
     */
    EdgeIterator::Ptr getEdgeIterator()
    {
        ArcIterator<DirectedGraph>* it = new ArcIterator<DirectedGraph>(*this);
        return EdgeIterator::Ptr(it);
    }

protected:
    // Property maps to store data associated with vertices and edges
    EdgeMap mEdgeMap;
    VertexMap mVertexMap;
};


} // end namespace lemon
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_LEMON_GRAPH_HPP
