#ifndef GRAPH_ANALYSIS_ALGORITHMS_DFS_VISITOR_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_DFS_VISITOR_HPP

#include "Visitor.hpp"
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace algorithms {

/**
 * \class DFSVisitor
 * \brief Special visitor for the DFS
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
class DFSVisitor : public Visitor
{
public:
    typedef shared_ptr<DFSVisitor> Ptr;

    virtual void initializeVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "initializeVertex" << vertex->toString(); }
    virtual void leafVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "leafVertex" << vertex->toString(); }

    virtual void examineEdge(Edge::Ptr& edge) { LOG_DEBUG_S << "examineEdge: " << edge->toString(); }
    virtual void treeEdge(Edge::Ptr& edge) { LOG_DEBUG_S << "treeEdge: " << edge->toString(); }
    virtual void forwardOrCrossEdge(Edge::Ptr& edge) { LOG_DEBUG_S << "forwardOrCrossEdge: " << edge->toString(); }
    virtual void backEdge(Edge::Ptr& edge) { LOG_DEBUG_S << "backEdge: " << edge->toString(); }

    /**
     * Invoked when a vertex is encoutered for the first time
     */
    virtual void discoverVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "discoverVertex: " << vertex->toString(); }
    virtual void finishVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "finishVertex: " << vertex->toString(); }

};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_DFS_VISITOR_HPP
