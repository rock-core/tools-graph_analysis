#ifndef GRAPH_ANALYSIS_ALGORITHMS_DFS_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_DFS_HPP

#include <vector>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/algorithms/DFSVisitor.hpp>

namespace graph_analysis {
namespace algorithms {

typedef function1<bool,Edge::Ptr> Skipper;

class DFS
{
public:
    DFS(BaseGraph::Ptr& graph, const DFSVisitor::Ptr& visitor, Skipper skipper = Skipper());

    void run(const Vertex::Ptr& startVertex = Vertex::Ptr());

private:
    BaseGraph::Ptr mpGraph;
    Vertex::Ptr mStartVertex;
    std::vector<Vertex::Ptr> mStack;
    Visitor::Ptr mpVisitor;
    Skipper mSkipper;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_DFS_HPP
