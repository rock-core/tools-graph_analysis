#ifndef GRAPH_ANALYSIS_ALGORITHMS_BFS_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_BFS_HPP

#include <queue>
#include "../BaseGraph.hpp"
#include "Skipper.hpp"
#include "BFSVisitor.hpp"

namespace graph_analysis {
namespace algorithms {

/**
  * \class BFS
  * \brief Implementation of Breadth-First-Search
  *¸\details This implements the breadth-first search algorithm, it is
  * based on the existing implementation of the algorithm in Boost
  */
class BFS
{
public:
    /**
      * \param graph Graph to search
      * \param visitor Visitor object, which will be called, when BFS visits a
      * vertex or edge
      * \param skipper Skipper object to defined, when an edge does not need to * be followed
      */
    BFS(const BaseGraph::Ptr& graph, const Visitor::Ptr& visitor, Skipper skipper = Skipper());

    /**
      * Start the algorithm beginning at the specified vertex
      * \param startVertex Start vertex of the search
      */
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
