#ifndef GRAPH_ANALYSIS_ALGORITHMS_DFS_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_DFS_HPP

#include <vector>
#include "../BaseGraph.hpp"
#include "DFSVisitor.hpp"

namespace graph_analysis {
namespace algorithms {

typedef function1<bool,const Edge::Ptr&> Skipper;

/**
  * \class DFS
  * \brief Implementation of Depth-First-Search
  *¸\details This implements the depth-first search algorithm, it is
  * based on the existing implementation of the algorithm in Boost
  */
class DFS
{
public:
    /**
      * \param graph Graph to search
      * \param visitor Visitor object, which will be called, when BFS visits a
      * vertex or edge
      * \param skipper Skipper object to defined, when an edge does not need to * be followed
      */
    DFS(const BaseGraph::Ptr& graph, const DFSVisitor::Ptr& visitor, Skipper skipper = Skipper());

    /**
      * Start the algorithm beginning at the specified vertex
      * \param startVertex Start vertex of the search
      */
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
