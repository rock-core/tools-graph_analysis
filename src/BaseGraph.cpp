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

}
