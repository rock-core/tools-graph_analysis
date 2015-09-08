#ifndef GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_MIN_COST_FLOW_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_ GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_MIN_COST_FLOW_HPP

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/algorithms/GLPKSolver.hpp>
#include <graph_analysis/algorithms/MultiCommodityEdge.hpp>
#include <graph_analysis/algorithms/MultiCommodityVertex.hpp>

namespace graph_analysis {
namespace algorithms {

// Currently, doxygen is not very fault tolerant in recovering from typos in formulas. It may be necessary to remove the file formula.repository that is written to the html directory to get rid of an incorrect formula.
// https://www.stack.nl/~dimitri/doxygen/manual/formulas.html
// add EXTRA_PACKAGES={amsmath} to Doxyfile.in to support math mode and \text in
// formulas

/**
 *
 * The multi-commodity min cost flow problem (MMCF) (Kennington, 1978)
 * handles the routing of commodities (different types of resources) through a network
 * which is subject to constraints:
 *  - supply restriction
 *  - transport capacity restriction
 *  - flow conversation (at transshipment nodes)
 *
 \f[ \min \sum_{k,m} c_{m}^{k} x_{m}^{k} \f]
 \f[
     \text{, where}
 \f]
 \f{eqnarray*}{
        G &=& (V,E)  \\
        K &=& \text{ number of commodities} \\
        k &=&         \text{ 1 \dots K } \\
        M  &=&        \text{ size of G, i.e. number of edges } |E(G)|\\
        m  &=&        \text{ 1 \dots M }\\
        e_{m}  &=&    \text{ egde between node i and node j, i.e. (i,j) } \\
        x_{m}^{k} &=& \text{ flow for commodity k in arc } e_{m} \\
        c_{m}^{k} &=& \text{ unit cost for commodity k in arc } e_{m} \\
        u_{m}^{k} &=& \text{ upper bound for flow of commodity k through edge m} \\
        l_{m}^{k} &=& \text{ lower bound for flow of commodity k through edge m} \\
        s_{k}     &=& \text{ source of commodity k, } s_{k} \in V \\
        t_{k}     &=& \text{ target of commodity k, } t_{k} \in V \\
        S_{k}^{+}     &=& \text{ supply of source node } s_{k} \\
        S_{k}^{-}     &=& \text{ demand of target node } t_{k} \\
 \f}
 \f[
 subject\text{ }to
 \f]
 \f[
     \sum_{e_{m} \in A_{n}} x_{m}^{k} - \sum_{e_{m} \in B_{n}} x_{m}^{k} = \begin{cases}
         S_{k}^{+}      & \text{if } n=s_{k} \\
         -S_{k}^{-}     & \text{if } n=t_{k}\text{~~~~~~~~~~, }\forall\text{ }n,k \\
         0         & \text{otherwise}
         \end{cases}
 \f]
 \f[
     \text{, where}
 \f]
 \f{eqnarray*}{
     B_{n} &=&  \{ e = (i,j) | j = n\text{ }i,j,n \in V\text{ }\forall e \in B_{n} \}\\
     A_{n} &=&  \{ e = (n,j) | i = n\text{ }i,j,n \in V\text{ }\forall e \in A_{n} \}\\
 \f}
 \f[
    \sum_{k} x_{m}^{k} \leq b_{b}\text{, } \forall m
 \f]
 \f[
    0 \leq x_{m}^{k} \leq u_{m}^{k}\text{, } \forall m,k
 \f]
 *
 * Solutions approaches vary from:
 *  - decomposition methods
 *   - price directive approaches
 *     - lagrangian relaxation
 *     - dantzig wolfe decomposition
 *   - resource directive decomposition
 *   - partitioning methods
 *     - using network simplex
 *     - bundle bases (spanning trees)
 * We use the same graph structure as for the MinCostFlow problem, 
 * but use multiedges, which have to channel the flow according
 * to the type of commodity
 *
 * Edge: 
 *  - general capacity of commodity
 *  - capacity per commodity: upper / lower bound
 *  - cost per commodity
 *
 * Vertex:
 *  - balance constraint
 *
 *  Remarks: 
 *    - introduce a vertex per capacity demand, i.e. solve the 
 *      min cost flow problem per capacity, adjust the lower/upper capacity bounds
 *
 *
 * We first solve this using a direct linear programming approach
 * (see recommendation by Held, Wolfe, Crowder 1974), since we
 * assume a small set of commodities ( <= 25) and transshipment nodes (< 50)
 *
 *
 * Edge needs to contain:
 *  - general capacity overall all commodities -- upper bound and redundant for
 *  multiedges
 *  - capacity per commodity upper bound
 *  - cost per commodity
 *
 *
 */
class MultiCommodityMinCostFlow : public GLPKSolver
{
public:
    typedef MultiCommodityEdge edge_t;
    typedef MultiCommodityVertex vertex_t;

    /**
     * \param graph that contains only (!) MultiCommodityVertices and
     * MultiCommodityEdges (!)
     * \param commodities number of commodities that need to be accounted for
     */
    MultiCommodityMinCostFlow(const BaseGraph::Ptr& graph, uint32_t commodities);

    virtual ~MultiCommodityMinCostFlow() {}

    int run();

private:
    uint32_t mCommodities;
    BaseGraph::Ptr mpGraph;
    std::vector<Edge::Ptr> mColumnToEdge;

    uint32_t mTotalNumberOfColumns;
    uint32_t mTotalNumberOfRows;

    int getColumnIndex(const Edge::Ptr& e, uint32_t commodity = 0);

};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_MIN_COST_FLOW_HPP
