#ifndef GRAPH_ANALYSIS_ALGORITHMS_DFS_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_DFS_HPP

#include <vector>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/algorithms/DFSVisitor.hpp>

namespace graph_analysis {
namespace algorithms {

typedef function1<bool,const Edge::Ptr&> Skipper;

/**
 *
 * \verbatim
 class PathConstructor : public graph_analysis::algorithm::DFSVisitor
 {
    bool invalidTransition(graph_analysis::Edge::Ptr& edge) { // check if it a valid transition and return true or false ... }
    void discoverVertex(graph_analysis::Vertex::Ptr& vertex) { // collect vertices ... }
 };

 BaseGraph::Ptr graph = BaseGraph::getInstance();
 ...
 graph->addVertex(rootVertex);
 ...

 PathConstructor::Ptr pathConstructor(new PathConstructor());
 boost::function1<bool, graph_analysis::Edge::Ptr> skipper = boost::bind(&PathConstructor::invalidTransition, pathConstructor,_1);
 DFS dfs(graph, pathConstructor, skipper);
 dfs.run(rootVertex);

 \endverbatim
 */
class DFS
{
public:
    DFS(const BaseGraph::Ptr& graph, const DFSVisitor::Ptr& visitor, Skipper skipper = Skipper());

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
