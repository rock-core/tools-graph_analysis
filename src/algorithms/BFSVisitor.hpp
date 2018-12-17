#ifndef GRAPH_ANALYSIS_ALGORITHMS_BFS_VISITOR_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_BFS_VISITOR_HPP

#include "Visitor.hpp"
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace algorithms {

/**
 * \class BFSVisitor
 * \brief Special visitor for the BFS implementing the default functions
 */
class BFSVisitor : public Visitor
{
public:
    typedef shared_ptr<BFSVisitor> Ptr;

    virtual void startVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "startVertex" << vertex->toString(); }
    virtual void initializeVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "initializeVertex" << vertex->toString(); }
    virtual void leafVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "leafVertex" << vertex->toString(); }
    /**
     * Invoked when a vertex is encoutered for the first time
     */
    virtual void discoverVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "discoverVertex: " << vertex->toString(); }
    virtual void finishVertex(Vertex::Ptr& vertex) { LOG_DEBUG_S << "finishVertex: " << vertex->toString(); }


    virtual void examineEdge(Edge::Ptr& edge) { LOG_DEBUG_S << "examineEdge: " << edge->toString(); }
    virtual void treeEdge(Edge::Ptr& edge) { LOG_DEBUG_S << "treeEdge: " << edge->toString(); }
    virtual void forwardOrCrossEdge(Edge::Ptr& edge) { LOG_DEBUG_S << "forwardOrCrossEdge: " << edge->toString(); }
    virtual void backEdge(Edge::Ptr& edge) { LOG_DEBUG_S << "backEdge: " << edge->toString(); }

};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_BFS_VISITOR_HPP
