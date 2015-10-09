#include "DirectedGraphInterface.hpp"
#include "EdgeIterator.hpp"
#include "Vertex.hpp"

using namespace graph_analysis;


SpecializedIterator<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr> DirectedGraphInterface::inEdges(Vertex::Ptr vertex) const
{
    return SpecializedIterator<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr>(this, vertex, &DirectedGraphInterface::getInEdgeIterator);
}

SpecializedIterator<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr> DirectedGraphInterface::outEdges(Vertex::Ptr vertex) const
{
    return SpecializedIterator<EdgeIterator::Ptr,DirectedGraphInterface, Edge::Ptr,Vertex::Ptr>(this, vertex, &DirectedGraphInterface::getOutEdgeIterator);
}
