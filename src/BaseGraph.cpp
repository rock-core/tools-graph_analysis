#include "BaseGraph.hpp"

namespace graph_analysis {

GraphId BaseGraph::msId = 0;

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
SubGraph::Ptr BaseGraph::applyFilters(Filter<Vertex::Ptr>::Ptr vertexFilter, Filter<Edge::Ptr>::Ptr edgeFilter)
{
    SubGraph::Ptr subGraph = getSubGraph();
    subGraph->applyFilters(vertexFilter, edgeFilter);
    return subGraph;
}

}
