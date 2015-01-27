#ifndef GRAPH_ANALYSIS_ALGORITHMS_CORRELATION_CLUSTERING_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_CORRELATION_CLUSTERING_HPP

#include <map>
#include <boost/function.hpp>
#include <glpk.h>
#include <numeric/Combinatorics.hpp>
#include <base/Logging.hpp>
#include <graph_analysis/EdgeIterator.hpp>
#include <graph_analysis/Graph.hpp>

namespace graph_analysis {
namespace algorithms {

typedef boost::function1<double, Edge::Ptr> EdgeWeightFunction;

struct Ball
{
    graph_analysis::BaseGraph::Ptr graph;
    double radius;
    double volume;
};

/**
 * \details
 * This is an implementation of the correlation clustering as described in:
 * "Correlation Clustering in General Weighted Graphs" (Demaine et al., 2005) and
 * "Correlation Clustering with partial information" (Demaine et al., 2004)
 *
 * This implementation however does not(!) use non-negative edge weights, but interpretes the sign
 * as label <+>/<-> to describe similarity.
 *
 * To solve the resulting integer program the GLPK (GNU Linear Programming Kit) is used
 * \verbatim
 \endverbatim
 *
 */
class CorrelationClustering
{
    graph_analysis::BaseGraph::Ptr mpGraph;
    EdgeWeightFunction mEdgeWeightFunction;

    std::map<Edge::Ptr, double> mEdgeActivation;
    std::vector<Edge::Ptr> mColumnToEdge;

    glp_prob* mpProblem;
    size_t mTotalNumberOfColumns;

    double mInitialVolume;
    double mConstant;

    void prepare();
    void solve();

public:
    CorrelationClustering(BaseGraph::Ptr graph, EdgeWeightFunction weightfunction);

    static std::string toString(const std::map<Edge::Ptr, double>& solution);

    void round();

    /**
     * Get the edge activation after performing the correlation clustering
     * \return edgemap to retrieve edge activation -- 0 means active, 1 means inactive, as defined
     * in the original algorithm
     */
    std::map<Edge::Ptr, double> getEdgeActivation() const { return mEdgeActivation; }

    /**
     * Compute cut for the set of nodes in ball which is part of the overall
     * graph
     * \details
     * "The cut of a set S of nodes, denoted by cut(S) is the weight of the
     * positive edges with exactly one endpoint in S.[..] The cur of a ball is
     * the cut induced by the set of vertices included in the ball."
     */
    double cut(Ball ball);

    double volume(Ball ball);

   /**
    * \details
    * "A ball B(u,r) of radius r around node u consists of all nodes v such that
    * x_uv <=r, the subgraph induced by these nodes, and the fraction
    * (r-x_uv)/x_vw of edges (v,w) with only edpoint v \in B(u,r)"
    */
    Ball createBall(Vertex::Ptr vertex, double radius);
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_CORRELATION_CLUSTERING_HPP
