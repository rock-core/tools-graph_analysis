#ifndef GRAPH_ANALYSIS_ALGORITHMS_VISITOR_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_VISITOR_HPP

#include <unordered_map>
#include "../Edge.hpp"

#define GA_ALGO_NI(X) throw std::runtime_error("graph_analysis::algorithms::Visitor: " X " not implemented");

namespace graph_analysis {
namespace algorithms {

/**
  * This is an implementation of an Vertex/Edge Visitor
  * Closely related implementation to
  * \see http://www.boost.org/doc/libs/1_57_0/libs/graph/doc/EventVisitor.html
  */
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

    /**
     * Called when the vertex is initialized
     */
    virtual void initializeVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("initializeVertex") }

    /**
     * Called when the start vertex is identified
     */
    virtual void startVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("startVertex") }

    /**
     * Called when a vertex is discovered
     */
    virtual void discoverVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("discoverVertex") }

    /**
     * Called when a vertex has been completely processed
     */
    virtual void finishVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("finishVertex") }

    /**
     * Called when a leaf vertex - so no outgoing edges  - is discovered
     */
    virtual void leafVertex(Vertex::Ptr& vertex) { GA_ALGO_NI("leafVertex") }

    /**
     * Called when examining an edge - after it has been checked whether to skip
     * the edge or not
     */
    virtual void examineEdge(Edge::Ptr& edge) { GA_ALGO_NI("examineEdge") }

    /**
     * Called when an edge leads to a previously unknown vertex
     */
    virtual void treeEdge(Edge::Ptr& edge) { GA_ALGO_NI("treeEdge") }

    /**
     * Called when a cycle edge has been detected
     */
    virtual void cycleEdge(Edge::Ptr& edge) { GA_ALGO_NI("cycleEdge") }

    /**
     * Called when an edge has been encountered that leads to an already
     * visited node
     */
    virtual void forwardOrCrossEdge(Edge::Ptr& edge) { GA_ALGO_NI("forwardOrCrossEdge") }

    /**
     * Called when an edge in the opposite direction has already been
     * encountered
     */
    virtual void backEdge(Edge::Ptr& edge) { GA_ALGO_NI("backEdge") }

    virtual void edgeRelaxed(Edge::Ptr& edge) { GA_ALGO_NI("edgeRelaxed") }
    virtual void edgeNotRelaxed(Edge::Ptr& edge) { GA_ALGO_NI("edgeNotRelaxed") }
    virtual void edgeMinimized(Edge::Ptr& edge) { GA_ALGO_NI("edgeMinimized") }
    virtual void edgeNotMinimized(Edge::Ptr& edge) { GA_ALGO_NI("edgeNotMinimized") }


private:
    std::unordered_map<Vertex::Ptr, Status> mVertexStatus;
    std::unordered_map<Edge::Ptr, Status> mEdgeStatus;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_DFS_VISITOR_HPP
