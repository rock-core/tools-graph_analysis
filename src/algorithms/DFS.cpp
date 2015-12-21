#include "DFS.hpp"

namespace graph_analysis {
namespace algorithms {

DFS::DFS(BaseGraph::Ptr& graph, const DFSVisitor::Ptr& visitor, Skipper skipper)
    : mpGraph(graph)
    , mpVisitor(visitor)
    , mSkipper(skipper)
{}

void DFS::run(const Vertex::Ptr& startVertex)
{
    if(mpGraph->empty())
    {
        throw std::invalid_argument("graph_analysis::algorithms::DFS::run: search failed since graph is empty");
    }

    if(startVertex)
    {
        mStartVertex = startVertex;
    } else {
        VertexIterator::Ptr vertexIt = mpGraph->getVertexIterator();
        if(vertexIt->next())
        {
            mStartVertex = vertexIt->current();
        }
    }
    mStack.push_back(mStartVertex);
    mpVisitor->setStatus(mStartVertex, Visitor::REGISTERED);
    mpVisitor->initializeVertex(mStartVertex);

    Vertex::Ptr vertex;
    while(!mStack.empty())
    {
        vertex = mStack.back();
        mStack.pop_back();

        EdgeIterator::Ptr edgeIt = mpGraph->getOutEdgeIterator(vertex);
        bool edges = false;
        while(edgeIt->next())
        {
            edges = true;
            Edge::Ptr edge = edgeIt->current();
            if(mSkipper && mSkipper(edge))
            {
                continue;
            }
            mpVisitor->examineEdge(edge);

            Vertex::Ptr targetVertex = edge->getTargetVertex();
            switch(mpVisitor->getStatus(targetVertex))
            {
                case Visitor::UNKNOWN:
                    mpVisitor->treeEdge(edge);
                    mStack.push_back(targetVertex);
                    mpVisitor->discoverVertex(targetVertex);
                    mpVisitor->setStatus(targetVertex, Visitor::REGISTERED);
                    break;
                case Visitor::REGISTERED:
                    mpVisitor->backEdge(edge);
                    break;
                case Visitor::VISITED:
                    mpVisitor->forwardOrCrossEdge(edge);
                    break;
                default:
                    break;
            }
        }

        mpVisitor->setStatus(vertex, Visitor::VISITED);
        if(!edges)
        {
            mpVisitor->leafVertex(vertex);
        }
        mpVisitor->finishVertex(vertex);
    }
}

} // end namespace algorithms
} // end namespace graph_analysis
