#include "BaseGraph.hpp"
#include <sstream>
#include <boost/assign.hpp>
#include <base/Logging.hpp>
#include <graph_analysis/boost_graph/DirectedGraph.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/snap/DirectedGraph.hpp>

namespace graph_analysis {

GraphId BaseGraph::msId = 0;

std::map<BaseGraph::ImplementationType, std::string> BaseGraph::ImplementationTypeTxt = boost::assign::map_list_of
    (BOOST_DIRECTED_GRAPH, "Boost Directed Graph")
    (LEMON_DIRECTED_GRAPH, "Lemon Directed Graph")
    (SNAP_DIRECTED_GRAPH, "SNAP Directed Graph")
    ;

BaseGraph::BaseGraph(ImplementationType type)
    : mId(msId++)
    , mImplementationType(type)
{}


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

SubGraph::Ptr BaseGraph::getSubGraph(Ptr graph)
{
    return graph->createSubGraph(graph);
}

GraphElementId BaseGraph::addVertex(Vertex::Ptr vertex)
{
    if(vertex->associated(getId()) )
    {
        throw std::runtime_error("BaseGraph: vertex already exists in this graph");
    }

    return 0;
}

void BaseGraph::removeVertex(Vertex::Ptr vertex)
{
    if(!vertex->associated(getId()) )
    {
        throw std::runtime_error("BaseGraph: vertex cannot be removed, since it does not exist in this graph");
    }
    vertex->disassociate(getId());
}

GraphElementId BaseGraph::addEdge(Edge::Ptr edge)
{
    if(edge->associated(getId()) )
    {
        throw std::runtime_error("BaseGraph: edge already exists in this graph");
    }

    Vertex::Ptr source = edge->getSourceVertex();
    Vertex::Ptr target = edge->getTargetVertex();

    if(!source || !target)
    {
        throw std::runtime_error("BaseGraph: cannot add edge, since it has no source and/or target vertex specified");
    } else {
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
        return addEdgeInternal(edge, getVertexId(source), getVertexId(target));
    } catch(...)
    {
        edge->disassociate(getId());
        throw;
    }
}

void BaseGraph::removeEdge(Edge::Ptr edge)
{
    if(!edge->associated(getId()) )
    {
        throw std::runtime_error("BaseGraph: edge cannot be removed, since it does not exist in this graph");
    }
    edge->disassociate(getId());
}

std::vector<Edge::Ptr> BaseGraph::getEdges(Vertex::Ptr source, Vertex::Ptr target)
{
    std::vector<Edge::Ptr> edges;
    EdgeIterator::Ptr edgeIt = getEdgeIterator(source);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        if(edge->getTargetVertex() == target)
        {
            edges.push_back(edge);
        }
    }

    return edges;
}


std::vector<Vertex::Ptr> BaseGraph::getAllVertices()
{
    std::vector<Vertex::Ptr> vertices;
    VertexIterator::Ptr vertexIt = getVertexIterator();
    while(vertexIt->next())
    {
        vertices.push_back( vertexIt->current() );
    }
    return vertices;
}

std::vector<Edge::Ptr> BaseGraph::getAllEdges()
{
    std::vector<Edge::Ptr> edges;
    EdgeIterator::Ptr edgeIt = getEdgeIterator();
    while(edgeIt->next())
    {
        edges.push_back( edgeIt->current() );
    }
    return edges;
}

bool BaseGraph::contains(Edge::Ptr edge) const
{
    try {
        getEdgeId(edge);
        return true;
    } catch(const std::runtime_error& e)
    {
        return false;
    }
}

bool BaseGraph::contains(Vertex::Ptr vertex) const
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
SubGraph::Ptr BaseGraph::applyFilters(BaseGraph::Ptr graph, Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter)
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

}
