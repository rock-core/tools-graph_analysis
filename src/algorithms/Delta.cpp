#include "Delta.hpp"

namespace graph_analysis {
namespace algorithms {

Delta::Delta(const BaseGraph::Ptr& graph1, const BaseGraph::Ptr& graph2)
    : mpSourceGraph(graph1), mpTargetGraph(graph2)
{}

void Delta::run()
{
	typedef BaseIterable<VertexIterator::Ptr, BaseGraph, Vertex::Ptr> IterableVertices;
	IterableVertices::Iterator vit = mpSourceGraph->vertices().begin();

	for(; vit != mpSourceGraph->vertices().end(); ++vit)
	{
		Vertex::Ptr vertex = *vit;
		if (!mpTargetGraph->contains(vertex))
			mRemoveVertices.push_back(vertex);
	}

	vit = mpTargetGraph->vertices().begin();

	for(; vit != mpTargetGraph->vertices().end(); ++vit)
	{
		Vertex::Ptr vertex = *vit;
		if (!mpSourceGraph->contains(vertex))
			mAddVertices.push_back(vertex);
	}

	typedef BaseIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr> IterableEdges;
	IterableEdges::Iterator edgeIt = mpSourceGraph->edges().begin();

	for(; edgeIt != mpSourceGraph->edges().end(); ++edgeIt)
	{
		Edge::Ptr edge = *edgeIt;
		if (!mpTargetGraph->contains(edge))
			mRemoveEdges.push_back(edge);
	}	

	edgeIt = mpTargetGraph->edges().begin();

	for(; edgeIt != mpTargetGraph->edges().end(); ++edgeIt)
	{
		Edge::Ptr edge = *edgeIt;
		if (!mpSourceGraph->contains(edge))
			mAddEdges.push_back(edge);
	}
}

BaseGraph::Ptr Delta::getNext()
{
	if (!mRemoveEdges.empty())
	{
		Edge::Ptr edge = mRemoveEdges.back();
		mRemoveEdges.pop_back();
		mpSourceGraph->removeEdge(edge);
	}
	else if (!mRemoveVertices.empty())
	{
		Vertex::Ptr vertex = mRemoveVertices.back();
		mRemoveVertices.pop_back();
		mpSourceGraph->removeVertex(vertex); 
	}
	else if (!mAddVertices.empty())
	{
		Vertex::Ptr vertex = mAddVertices.back();
		mAddVertices.pop_back();
		mpSourceGraph->addVertex(vertex);
	}
	else if (!mAddEdges.empty())
	{
		Edge::Ptr edge = mAddEdges.back();
		mAddEdges.pop_back();
		mpSourceGraph->addEdge(edge);
	}
	else
	{
		throw std::runtime_error("graph_analysis::algorithms::Delta::getNext: next graph does not exist");
	}
	return mpSourceGraph;
}

bool Delta::hasNext()
{
	if (mRemoveEdges.empty() && mRemoveVertices.empty() && mAddVertices.empty() && mAddEdges.empty()) return false;
	return true;
}

} // end namespace algorithms
} // end namespace graph_analysis
