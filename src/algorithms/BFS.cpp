#include "BFS.hpp"

namespace graph_analysis {
namespace algorithms {

BFS::BFS(const BaseGraph::Ptr& graph, const BFSVisitor::Ptr& visitor, Skipper skipper)
    : mpGraph(graph)
    , mpVisitor(visitor)
    , mSkipper(skipper)
{}

void BFS::run(const Vertex::Ptr& startVertex)
{
    if(mpGraph->empty())
    {
        throw std::invalid_argument("graph_analysis::algorithms::BFS::run: search failed since graph is empty");
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
    mStack.push(mStartVertex);
    mpVisitor->setStatus(mStartVertex, Visitor::REGISTERED);
    mpVisitor->initializeVertex(mStartVertex);

    Vertex::Ptr vertex;

    //std::stringstream ss;
    while(!mStack.empty())
    {
        vertex = mStack.front();
        mStack.pop();

        //ss << vertex->toString() << " ";

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
                    mStack.push(targetVertex);
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
    //throw std::runtime_error(ss.str());
}

} // end namespace algorithms
} // end namespace graph_analysis
