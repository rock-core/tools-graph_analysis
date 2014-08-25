#include "Graph.hpp"

namespace graph_analysis {
namespace lemon {

DirectedSubGraph::DirectedSubGraph(DirectedGraph& graph)
    : TypedSubGraph(new VertexFilterType(graph.raw()), new EdgeFilterType(graph.raw()))
    , mGraph(graph)
{
     setSubgraph( new GraphType(graph.raw(), getVertexFilter(), getEdgeFilter()) );
}

void DirectedSubGraph::enable(Vertex::Ptr vertex)
{
    GraphElementId graphElementId = vertex->getId( mGraph.getId() );
    ::lemon::ListDigraph::Node n = mGraph.raw().nodeFromId( graphElementId );

    raw().enable(n);
}

void DirectedSubGraph::disable(Vertex::Ptr vertex)
{
    GraphElementId graphElementId = vertex->getId( mGraph.getId() );
    ::lemon::ListDigraph::Node node = mGraph.raw().nodeFromId( graphElementId );

    raw().disable(node);
}


void DirectedSubGraph::enable(Edge::Ptr edge)
{
    GraphElementId graphElementId = edge->getId( mGraph.getId() );
    ::lemon::ListDigraph::Arc arc = mGraph.raw().arcFromId( graphElementId );

    raw().enable(arc);
}

void DirectedSubGraph::disable(Edge::Ptr edge)
{
    GraphElementId graphElementId = edge->getId( mGraph.getId() );
    ::lemon::ListDigraph::Arc arc = mGraph.raw().arcFromId( graphElementId );

    raw().disable(arc);
}

bool DirectedSubGraph::enabled(Vertex::Ptr vertex) const
{
    GraphElementId graphElementId = vertex->getId( mGraph.getId() );
    ::lemon::ListDigraph::Node n = mGraph.raw().nodeFromId( graphElementId );
    bool status = raw().status(n);
    LOG_DEBUG_S << "Vertex " << vertex->toString() << " has status : " << status;
    return status;
}

bool DirectedSubGraph::enabled(Edge::Ptr edge) const
{
    GraphElementId graphElementId = edge->getId( mGraph.getId() );
    ::lemon::ListDigraph::Arc arc = mGraph.raw().arcFromId( graphElementId );
    return raw().status(arc);
}


DirectedGraph::DirectedGraph()
    : TypedGraph()
    , mEdgeMap(raw())
    , mVertexMap(raw())
{}

DirectedGraph::DirectedGraph(const DirectedGraph& other)
    : TypedGraph()
    , mEdgeMap(raw())
    , mVertexMap(raw())
{
    *this = other;
}

GraphElementId DirectedGraph::addVertex(Vertex::Ptr vertex)
{
    BaseGraph::addVertex(vertex);

    GraphType::Node node = mGraph.addNode();
    int nodeId = mGraph.id(node);
    mVertexMap[node] = vertex;

    vertex->associate(getId(), nodeId);
    return nodeId;
}

GraphElementId DirectedGraph::addEdgeInternal(Edge::Ptr edge, GraphElementId sourceVertexId, GraphElementId targetVertexId)
{
    GraphType::Node sourceNode = mGraph.nodeFromId( sourceVertexId );
    GraphType::Node targetNode = mGraph.nodeFromId( targetVertexId );

    GraphType::Arc arc = mGraph.addArc(sourceNode, targetNode);
    int arcId = mGraph.id(arc);
    edge->associate(getId(), arcId);
    mEdgeMap[arc] = edge;

    return arcId;
}

Vertex::Ptr DirectedGraph::getVertex(GraphElementId id) const
{
    return mVertexMap[ mGraph.nodeFromId(id) ];
}

Edge::Ptr DirectedGraph::getEdge(GraphElementId id) const
{
    return mEdgeMap[ mGraph.arcFromId(id) ];
}

Vertex::Ptr DirectedGraph::getSourceVertex(Edge::Ptr e) const
{
    GraphElementId edgeId = getEdgeId(e);
    return mVertexMap[ mGraph.source( mGraph.arcFromId(edgeId)) ];
}

Vertex::Ptr DirectedGraph::getTargetVertex(const Edge::Ptr& e) const
{
    GraphElementId edgeId = getEdgeId(e);
    return mVertexMap[ mGraph.target( mGraph.arcFromId(edgeId)) ];
}

void DirectedGraph::removeVertex(Vertex::Ptr vertex)
{
    BaseGraph::removeVertex(vertex);

    int nodeId = getVertexId( vertex );
    GraphType::Node node = mGraph.nodeFromId(nodeId);
    mGraph.erase(node);
}

void DirectedGraph::removeEdge(Edge::Ptr edge)
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
DirectedGraph& DirectedGraph::operator=(const DirectedGraph& other)
{
    ::lemon::digraphCopy(other.mGraph, this->mGraph).
        nodeMap(other.mVertexMap, this->mVertexMap).
        arcMap(other.mEdgeMap, this->mEdgeMap).
        run();

    for( GraphType::NodeIt n(this->mGraph); n != ::lemon::INVALID; ++n)
    {
        Vertex::Ptr vertex = mVertexMap[n];
        vertex->associate(this->getId(), this->mGraph.id(n));
    }

    for( GraphType::ArcIt a(this->mGraph); a != ::lemon::INVALID; ++a)
    {
        Edge::Ptr edge = mEdgeMap[a];
        edge->associate(this->getId(), this->mGraph.id(a));
    }

    return *this;
}

DirectedSubGraph DirectedGraph::applyFilters(Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter)
{
    DirectedSubGraph subgraph(*this);

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
            Edge::Ptr edge = mEdgeMap[a];

            // By default enable all edges
            subgraph.raw().enable(a);

            EdgeContextFilter::Ptr contextFilter = boost::dynamic_pointer_cast<EdgeContextFilter>(edgeFilter);
            if( edgeFilter->evaluate(edge) )
            {
                // A context filter should apply to source / target nodes -- no need to filter this edge specifically then
                if(contextFilter)
                {
                    if(contextFilter->filterTarget(edge))
                    {
                        subgraph.raw().disable( mGraph.target(a));
                    }

                    if(contextFilter->filterSource(edge))
                    {
                        subgraph.raw().disable( mGraph.source(a));
                    }
                } else {
                    subgraph.raw().disable(a);
                }
            }
        }
    }

    return subgraph;
}

void DirectedGraph::write(std::ostream& ostream) const
{
    // Workaround:
    // operator<
    // will be overloaded ambiguously due to using shared_ptr
    // Use explicit conversion to string map first

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

VertexIterator::Ptr DirectedGraph::getVertexIterator()
{
    NodeIterator<DirectedGraph>* it = new NodeIterator<DirectedGraph>(*this);
    return VertexIterator::Ptr(it);
}

EdgeIterator::Ptr DirectedGraph::getEdgeIterator()
{
    ArcIterator<DirectedGraph>* it = new ArcIterator<DirectedGraph>(*this);
    return EdgeIterator::Ptr(it);
}

EdgeIterator::Ptr DirectedGraph::getOutEdgeIterator(Vertex::Ptr vertex)
{
    OutArcIterator<DirectedGraph>* it = new OutArcIterator<DirectedGraph>(*this, vertex);
    return EdgeIterator::Ptr(it);
}

EdgeIterator::Ptr DirectedGraph::getInEdgeIterator(Vertex::Ptr vertex)
{
    InArcIterator<DirectedGraph>* it = new InArcIterator<DirectedGraph>(*this, vertex);
    return EdgeIterator::Ptr(it);
}

DirectedGraph::SubGraph DirectedGraph::identifyConnectedComponents(DirectedGraph& graph, DirectedGraph::SubGraph& subgraph)
{
    ::lemon::Undirector<DirectedGraph::GraphType> undirected(graph.raw());
    // Identify the components
    GraphType::NodeMap<int> nodeMap(graph.raw(),false);
    int componentCount = ::lemon::connectedComponents(undirected, nodeMap);

    // Add a single vertex per identified component
    // activate that node in the subgraph
    Vertex::Ptr components[componentCount];
    for(int i = 0; i < componentCount; ++i)
    {
        Vertex::Ptr vertex(new Vertex());
        components[i] = vertex;

        // Activate this node in the subgraph
        // disabling all other will be in the next loop
        GraphElementId id = graph.addVertex(vertex);
        subgraph.raw().enable( graph.raw().nodeFromId( id ) );
    }

    if(componentCount > 0)
    {
        // Disable all nodes in the subgraph that are not representing a component
        // Add an edge to relate vertices to components
        for(DirectedSubGraph::GraphType::NodeIt n(subgraph.raw()); n != ::lemon::INVALID; ++n)
        {
            bool isComponentNode = false;
            Vertex::Ptr sourceVertex = mVertexMap[n];
            for(int a = 0; a < componentCount; ++a)
            {
                if(sourceVertex->getUid() == components[a]->getUid())
                {
                    isComponentNode = true;
                }
            }

            if(isComponentNode)
            {
                continue;
            }

            Vertex::Ptr targetVertex = components[ nodeMap[n] ];
            subgraph.raw().disable(n);
            Edge::Ptr edge( new Edge(sourceVertex, targetVertex) );
            graph.addEdge(edge);
        }
    } else {
        LOG_DEBUG("no component found in graph");
    }

    return subgraph;
}

} // end namespace lemon
} // end namespace graph_analysis
