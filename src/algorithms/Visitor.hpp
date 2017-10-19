#ifndef GRAPH_ANALYSIS_ALGORITHMS_VISITOR_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_VISITOR_HPP

#include <map>
#include "../Edge.hpp"

#define GA_ALGO_NI(X) throw std::runtime_error("graph_analysis::algorithms::Visitor: " X " not implemented");

namespace graph_analysis {
namespace algorithms {

// Closely related implementation to
// http://www.boost.org/doc/libs/1_57_0/libs/graph/doc/EventVisitor.html
//
class Visitor
{
public:
    typedef shared_ptr<Visitor> Ptr;

    enum Status { UNKNOWN = 0, REGISTERED, VISITED };

    virtual ~Visitor() {}

    void setStatus(const Vertex::Ptr& vertex, Status status) { mVertexStatus[vertex] = status; }
    void setStatus(const Edge::Ptr& edge, Status status) { mEdgeStatus[edge] = status; }

    Status getStatus(const Vertex::Ptr& vertex) const;
    Status getStatus(const Edge::Ptr& edge) const;


    virtual void initializeVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("initializeVertex") }
    virtual void startVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("startVertex") }
    virtual void discoverVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("discoverVertex") }
    virtual void finishVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("finishVertex - la") }
    virtual void leafVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("leafVertex") }

    virtual void examineEdge(Edge::Ptr& edge) { GA_ALGO_NI("examineEdge") }
    virtual void treeEdge(Edge::Ptr& edge) { GA_ALGO_NI("treeEdge") }
    virtual void cycleEdge(Edge::Ptr& edge) { GA_ALGO_NI("cycleEdge") }
    virtual void forwardOrCrossEdge(Edge::Ptr& edge) { GA_ALGO_NI("forwardOrCrossEdge") }
    virtual void backEdge(Edge::Ptr& edge) { GA_ALGO_NI("backEdge") }
    virtual void edgeRelaxed(Edge::Ptr& edge) { GA_ALGO_NI("edgeRelaxed") }
    virtual void edgeNotRelaxed(Edge::Ptr& edge) { GA_ALGO_NI("edgeNotRelaxed") }
    virtual void edgeMinimized(Edge::Ptr& edge) { GA_ALGO_NI("edgeMinimized") }
    virtual void edgeNotMinimized(Edge::Ptr& edge) { GA_ALGO_NI("edgeNotMinimized") }


private:
    std::map<Vertex::Ptr, Status> mVertexStatus;
    std::map<Edge::Ptr, Status> mEdgeStatus;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_DFS_VISITOR_HPP
