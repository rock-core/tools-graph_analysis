#include "BaseGraph.hpp"
#include <base-logging/Logging.hpp>
#include <sstream>
#include "boost_graph/DirectedGraph.hpp"
#include "lemon/Graph.hpp"
#include "snap/DirectedGraph.hpp"
#include "MapInitializer.hpp"

namespace graph_analysis {

GraphId BaseGraph::msId = 0;

std::map<BaseGraph::ImplementationType, std::string> BaseGraph::ImplementationTypeTxt = InitMap<BaseGraph::ImplementationType, std::string>
    (BOOST_DIRECTED_GRAPH, "boost_graph::DirectedGraph")
    (LEMON_DIRECTED_GRAPH, "lemon::DirectedGraph")
    (SNAP_DIRECTED_GRAPH, "snap::DirectedGraph")
    ;

BaseGraph::BaseGraph(ImplementationType type, bool directed)
    : mId(msId++)
    , mImplementationType(type)
    , mDirected(directed)
{
}

BaseGraph::Ptr BaseGraph::getInstance(ImplementationType type)
{
   BaseGraph::Ptr baseGraph;
   switch(type)
   {
       case BOOST_DIRECTED_GRAPH:
            return BaseGraph::Ptr(new graph_analysis::boost_graph::DirectedGraph());
       case LEMON_DIRECTED_GRAPH:
           return BaseGraph::Ptr(new graph_analysis::lemon::DirectedGraph());
       case SNAP_DIRECTED_GRAPH:
           return BaseGraph::Ptr(new graph_analysis::snap::DirectedGraph());
       default:
           std::stringstream ss;
           ss << type;
           throw std::invalid_argument("BaseGraph::getInstance: requested instanciation of unknown \
                   implementation type: " + ss.str());
   }
}

void BaseGraph::addObserver(const BaseGraphObserver::Ptr& observer)
{
    mObservers.insert(observer);
}

void BaseGraph::removeObserver(const BaseGraphObserver::Ptr& observer)
{
    mObservers.erase(observer);
}

void BaseGraph::notifyAll(const Vertex::Ptr& vertex, const EventType& event)
{
    std::set<BaseGraphObserver::Ptr>::const_iterator it;

    // Call every registered observer
    for(it = mObservers.begin(); it != mObservers.end(); ++it)
    {
        (*it)->notify(vertex, event, getId());
    }
}

void BaseGraph::notifyAll(const Edge::Ptr& edge, const EventType& event)
{
    std::set<BaseGraphObserver::Ptr>::const_iterator it;

    // Call every registered observer
    for(it = mObservers.begin(); it != mObservers.end(); ++it)
    {
        (*it)->notify(edge, event, getId());
    }
}

void BaseGraph::notifyAll(const TransactionType& event)
{
    std::set<BaseGraphObserver::Ptr>::const_iterator it;

    // Call every registered observer
    for(it = mObservers.begin(); it != mObservers.end(); ++it)
    {
        (*it)->notify(event, getId());
    }
}

void BaseGraph::transactionEvent( TransactionType eventType )
{
   notifyAll( eventType );
}

BaseGraph::Ptr BaseGraph::clone() const
{
    std::map<Vertex::Ptr, Vertex::Ptr> current2Clone;

    BaseGraph::Ptr g_clone = this->newInstance();
    VertexIterator::Ptr vertexIterator = getVertexIterator();
    while(vertexIterator->next())
    {
        Vertex::Ptr v = vertexIterator->current();
        Vertex::Ptr v_clone = v->clone();

        g_clone->addVertex(v_clone);
        current2Clone[v] = v_clone;
    }

    EdgeIterator::Ptr edgeIterator = getEdgeIterator();
    while(edgeIterator->next())
    {
        Edge::Ptr e = edgeIterator->current();
        Edge::Ptr e_clone = e->clone();
        std::map<Vertex::Ptr, Vertex::Ptr>::const_iterator vit;

        vit = current2Clone.find(e->getSourceVertex());
        if(vit != current2Clone.end())
        {
            e_clone->setSourceVertex(vit->second);
        } else {
            throw std::runtime_error("graph_analysis::BaseGraph::clone: could not find mapped source vertex -- internal error");
        }

        vit = current2Clone.find(e->getTargetVertex());
        if(vit != current2Clone.end())
        {
            e_clone->setTargetVertex(vit->second);
        } else {
            throw std::runtime_error("graph_analysis::BaseGraph::clone: could not find mapped target vertex -- internal error");
        }
        g_clone->addEdge(e_clone);
    }

    return g_clone;
}

BaseGraph::Ptr BaseGraph::cloneEdges() const
{
    std::map<Vertex::Ptr, Vertex::Ptr> current2Clone;

    BaseGraph::Ptr g_clone = this->newInstance();
    VertexIterator::Ptr vertexIterator = getVertexIterator();
    while(vertexIterator->next())
    {
        Vertex::Ptr v = vertexIterator->current();
        g_clone->addVertex(v);
    }

    EdgeIterator::Ptr edgeIterator = getEdgeIterator();
    while(edgeIterator->next())
    {
        Edge::Ptr e = edgeIterator->current();
        Edge::Ptr e_clone = e->clone();
        e_clone->setSourceVertex(e->getSourceVertex());
        e_clone->setTargetVertex(e->getTargetVertex());
        g_clone->addEdge(e_clone);
    }

    return g_clone;
}

SubGraph::Ptr BaseGraph::getSubGraph(const Ptr& graph)
{
    return graph->createSubGraph(graph);
}

GraphElementId BaseGraph::addVertex(const Vertex::Ptr& vertex)
{
    if(vertex->associated(getId()) )
    {
        throw std::runtime_error("BaseGraph: vertex already exists in this graph");
    }

    GraphElementId vertexId = addVertexInternal(vertex);

    // Call observers
    notifyAll(vertex, EVENT_TYPE_ADDED);

    return vertexId;
}

void BaseGraph::removeVertex(const Vertex::Ptr& vertex)
{
    if(!vertex->associated(getId()) )
    {
        throw std::runtime_error("BaseGraph: vertex cannot be removed, since it does not exist in this graph");
    }
    removeVertexInternal(vertex);
    vertex->disassociate(getId());

    // Call observers
    notifyAll(vertex, EVENT_TYPE_REMOVED);
}

GraphElementId BaseGraph::addEdge(const Edge::Ptr& edge)
{
    if(edge->associated(getId()))
    {
        throw std::runtime_error("BaseGraph: edge '" + edge->toString() +
                                 "' already exists in this graph");
    }

    Vertex::Ptr source = edge->getSourceVertex();
    Vertex::Ptr target = edge->getTargetVertex();

    if(!source)
    {
        throw std::runtime_error("BaseGraph: cannot add edge '" +
                                 edge->toString() +
                                 "' since it has no source vertex specified");
    }
    else if(!target)
    {
        throw std::runtime_error("BaseGraph: cannot add edge '" +
                                 edge->toString() +
                                 "' since it has no target vertex specified");
    }
    else
    {
        try {
            addVertex(source);
        } catch(const std::runtime_error& e)
        {
            // vertex already part of the graph
        }

        try {
            addVertex(target);
        } catch(const std::runtime_error& e)
        {
            // vertex already part of the graph
        }
    }

    try {
        GraphElementId retVal =
            addEdgeInternal(edge, getVertexId(source), getVertexId(target));

        // Call observers
        notifyAll(edge, EVENT_TYPE_ADDED);

        return retVal;
    } catch(...)
    {
        edge->disassociate(getId());
        throw;
    }
}

void BaseGraph::removeEdge(const Edge::Ptr& edge)
{
    if(!edge->associated(getId()) )
    {
        throw std::runtime_error("BaseGraph: edge cannot be removed, since it does not exist in this graph");
    }
    removeEdgeInternal(edge);
    edge->disassociate(getId());

    // Call observers
    notifyAll(edge, EVENT_TYPE_REMOVED);
}

std::vector<Edge::Ptr> BaseGraph::getEdges(const Vertex::Ptr& source, const Vertex::Ptr& target) const
{
    std::vector<Edge::Ptr> edges;
    EdgeIterator::Ptr edgeIt;
    if(isDirected())
    {
        edgeIt = getInEdgeIterator(target);
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            if(edge->getSourceVertex() == source)
            {
                edges.push_back(edge);
            }
        }
    } else {
        edgeIt = getEdgeIterator(target);
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            if(edge->getSourceVertex() == source || edge->getTargetVertex() == source)
            {
                edges.push_back(edge);
            }
        }
    }
    return edges;
}

size_t BaseGraph::removeEdges(const Vertex::Ptr& a, const Vertex::Ptr& b)
{
    size_t numberOfEdges = 0;

    std::vector<Edge::Ptr> edgesAB = getEdges(a, b);
    std::vector<Edge::Ptr> edgesBA = getEdges(b, a);

    {
        std::vector<Edge::Ptr>::const_iterator cit = edgesAB.begin();
        for(; cit != edgesAB.end(); ++cit)
        {
            removeEdge(*cit);
            ++numberOfEdges;
        }
    }
    {
        std::vector<Edge::Ptr>::const_iterator cit = edgesBA.begin();
        for(; cit != edgesBA.end(); ++cit)
        {
            removeEdge(*cit);
            ++numberOfEdges;
        }
    }
    return numberOfEdges;
}

std::vector<Vertex::Ptr> BaseGraph::getAllVertices() const
{
    std::vector<Vertex::Ptr> vertices;
    VertexIterator::Ptr vertexIt = getVertexIterator();
    while(vertexIt->next())
    {
        vertices.push_back( vertexIt->current() );
    }
    return vertices;
}

std::vector<Edge::Ptr> BaseGraph::getAllEdges() const
{
    std::vector<Edge::Ptr> edges;
    EdgeIterator::Ptr edgeIt = getEdgeIterator();
    while(edgeIt->next())
    {
        edges.push_back( edgeIt->current() );
    }
    return edges;
}

bool BaseGraph::contains(const Edge::Ptr& edge) const
{
    try {
        getEdgeId(edge);
        return true;
    } catch(const std::runtime_error& e)
    {
        return false;
    }
}

bool BaseGraph::contains(const Vertex::Ptr& vertex) const
{
    try {
        getVertexId(vertex);
        return true;
    } catch(const std::runtime_error& e)
    {
        return false;
    }
}

/**
 * Apply filters to base graph
 */
SubGraph::Ptr BaseGraph::applyFilters(const BaseGraph::Ptr& graph, const Filter<Vertex::Ptr>::Ptr& vertexFilter, const Filter<Edge::Ptr>::Ptr& edgeFilter)
{
    LOG_DEBUG_S << "Applying filters";
    SubGraph::Ptr subGraph = BaseGraph::getSubGraph(graph);
    LOG_DEBUG_S << "Got subgraph";
    subGraph->applyFilters(vertexFilter, edgeFilter);
    LOG_DEBUG_S << "return subgraph";
    return subGraph;
}


void BaseGraph::clear()
{
    while(true)
    {
        EdgeIterator::Ptr edgeIterator = getEdgeIterator();
        if(edgeIterator->next())
        {
            removeEdge(edgeIterator->current());
        } else {
            break;
        }
    }

    while(true)
    {
        VertexIterator::Ptr vertexIterator = getVertexIterator();
        if(vertexIterator->next())
        {
            removeVertex(vertexIterator->current());
        } else {
            break;
        }
    }
}

bool BaseGraph::empty() const
{
    VertexIterator::Ptr vertexIt = getVertexIterator();
    return !vertexIt->next();
}

std::vector<Edge::Ptr> BaseGraph::getInEdges(const Vertex::Ptr& vertex) const
{
    std::vector<Edge::Ptr> edges;
    EdgeIterator::Ptr edgeIt = getInEdgeIterator(vertex);
    while(edgeIt->next())
    {
        edges.push_back(edgeIt->current());
    }
    return edges;
}

std::vector<Edge::Ptr> BaseGraph::getOutEdges(const Vertex::Ptr& vertex) const
{
    std::vector<Edge::Ptr> edges;
    EdgeIterator::Ptr edgeIt = getOutEdgeIterator(vertex);
    while(edgeIt->next())
    {
        edges.push_back(edgeIt->current());
    }
    return edges;
}


SpecializedIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr,Vertex::Ptr> BaseGraph::inEdges(const Vertex::Ptr& vertex) const
{
    return SpecializedIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr,Vertex::Ptr>(this, vertex, &BaseGraph::getInEdgeIterator);
}

SpecializedIterable<EdgeIterator::Ptr,BaseGraph, Edge::Ptr,Vertex::Ptr> BaseGraph::outEdges(const Vertex::Ptr& vertex) const
{
    return SpecializedIterable<EdgeIterator::Ptr,BaseGraph, Edge::Ptr,Vertex::Ptr>(this, vertex, &BaseGraph::getOutEdgeIterator);
}

}
