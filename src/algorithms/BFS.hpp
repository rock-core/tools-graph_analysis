#ifndef GRAPH_ANALYSIS_ALGORITHMS_BFS_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_BFS_HPP

#include <queue>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/algorithms/BFSVisitor.hpp>

namespace graph_analysis {
namespace algorithms {

typedef function1<bool,Edge::Ptr> Skipper;

class BFS
{
public:
    BFS(const BaseGraph::Ptr& graph, const BFSVisitor::Ptr& visitor, Skipper skipper = Skipper());

    void run(const Vertex::Ptr& startVertex = Vertex::Ptr());

private:
    BaseGraph::Ptr mpGraph;
    Vertex::Ptr mStartVertex;
    std::queue<Vertex::Ptr> mStack;
    Visitor::Ptr mpVisitor;
    Skipper mSkipper;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_BFS_HPP
