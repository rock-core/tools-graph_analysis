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
 *
 */
typedef boost::function1<double, Edge::Ptr> EdgeWeightFunction;

class CorrelationClustering
{
    graph_analysis::BaseGraph* mpGraph;
    EdgeWeightFunction mEdgeWeightFunction;

    std::map<Edge::Ptr, double> mEdgeActivation;
    std::vector<Edge::Ptr> mColumnToEdge;

    glp_prob* mpProblem;
    size_t mTotalNumberOfColumns;

    void prepare();
    void solve();
    void round();

public:
    CorrelationClustering(BaseGraph* graph, EdgeWeightFunction weightfunction);

    static std::string toString(const std::map<Edge::Ptr, double>& solution);

    /**
     * Get the edge activation after performing the correlation clustering
     * \return edgemap to retrieve edge activation -- 0 means active, 1 means inactive, as defined
     * in the original algorithm
     */
    std::map<Edge::Ptr, double> getEdgeActivation() const { return mEdgeActivation; }
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_CORRELATION_CLUSTERING_HPP
