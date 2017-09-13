#ifndef GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_MIN_COST_FLOW_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_MIN_COST_FLOW_HPP

#include "../BaseGraph.hpp"
#include "../GraphIO.hpp"
#include "GLPKSolver.hpp"
#include "MultiCommodityEdge.hpp"
#include "MultiCommodityVertex.hpp"
#include "ConstraintViolation.hpp"

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
 * Usage example:
 * \verbatim

        BaseGraph::Ptr graph = BaseGraph::getInstance();
        uint32_t commodities = 3;
        uint32_t edgeCapacityUpperBound = 60;

        ...
        MultiCommodityMinCostFlow::vertex_t::Ptr v0( new MultiCommodityMinCostFlow::vertex_t(commodities));
        MultiCommodityMinCostFlow::vertex_t::Ptr v1( new MultiCommodityMinCostFlow::vertex_t(commodities));

        for(size_t i = 0; i < commodities; ++i)
        {
            // Source node
            v0->setCommoditySupply(i, 50);
            // Target node
            v1->setCommoditySupply(i, -50);
        }
        graph->addVertex(v0);
        graph->addVertex(v1);

        // Edges
        MultiCommodityMinCostFlow::edge_t::Ptr e0(new MultiCommodityMinCostFlow::edge_t(commodities));
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);
        e0->setCapacityUpperBound(edgeCapacityUpperBound);
        for(size_t i = 0; i < commodities; ++i)
        {
            e0->setCommodityCapacityUpperBound(i, 20);
        }
        graph->addEdge(e0);


        MultiCommodityMinCostFlow minCostFlow(graph, commodities);
        uint32_t cost = minCostFlow.run();

        std::string file("/tmp/algorithm-multicommodity-mincostflow-2.");
        minCostFlow.saveProblem(file + "problem");
        minCostFlow.saveSolution(file + "solution");

        minCostFlow.storeResult();
        EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
        while(edgeIt->next())
        {
            MultiCommodityMinCostFlow::edge_t::Ptr edge = dynamic_pointer_cast<MultiCommodityMinCostFlow::edge_t>( edgeIt->current() );
            if(edge)
            {
                std::cout << "Flows on: " << edge->toString() << ": id " << graph->getEdgeId(edge) << std::endl;

                for(size_t i = 0; i < commodities; ++i)
                {
                    std::cout << "    commodity #" << i << ": " << edge->getCommodityFlow(i) << std::endl;
                }
            }
        }
        std::cout << "Final objective value: " << minCostFlow.getObjectiveValue();
 \endverbatim
 *
 */
class MultiCommodityMinCostFlow : public GLPKSolver
{
public:
    typedef MultiCommodityEdge edge_t;
    typedef MultiCommodityVertex vertex_t;

    /**
     * Create a MultiCommodityMinCostFlow problem by loading (problem graph) from file
     */
    static MultiCommodityMinCostFlow fromFile(const std::string& filename,
            representation::Type format = representation::UNKNOWN,
            LPSolver::Type solverType = GLPK_SOLVER);

    /**
     * Save the MultiCommodityMinCostFlow problem to a file
     */
    void save(const std::string& filename, representation::Type format = representation::GEXF);

    /**
     * \param graph that contains only (!) MultiCommodityVertices and
     * MultiCommodityEdges (!)
     * \param commodities number of commodities that need to be accounted for,
     * if 0 is given then information is extracted from the edges
     */
    MultiCommodityMinCostFlow(const BaseGraph::Ptr& graph,
            uint32_t commodities = 0,
            LPSolver::Type solverType = GLPK_SOLVER);

    virtual ~MultiCommodityMinCostFlow() {}

    /**
     * Creates the problem instance and return the temporary file in which the
     * problem is saved
     */
    std::string createProblem(LPSolver::ProblemFormat format = CPLEX);

    /**
     * Solve the multicommodity problem with the given LP solver
     * save the solution to the given filename (or a temporary file),
     * and store the flow status into the base graph with with which the problem
     * has been initalized
     * \param solutionFile the filename to save the solution
     */
    LPSolver::Status solve(const std::string& solutionFile = "");

    /**
     * Save the solution to a given file and format
     * \param lp_problem_file problem file
     * \param lp_solution_file filename of the solution file
     * \param format format of the solution
     */
    void storeResult();

    // Validate the result and return the number of violated inflow constraints
    std::vector<ConstraintViolation> validateInflow() const;
private:
    LPSolver::Ptr mpSolver;

    /// Number of commodities -- in order to identify the flow of a each
    /// individual commodity instance, the vector of commodities has
    /// as length the number of instances of goods
    uint32_t mCommodities;
    /// The graph defining the overall flow network
    BaseGraph::Ptr mpGraph;
    /// Map a column to a particular edge
    std::vector<Edge::Ptr> mColumnToEdge;

    /// The total number of columns of the linear problem
    uint32_t mTotalNumberOfColumns;
    /// The total number of rows of the linear problem
    uint32_t mTotalNumberOfRows;

    /**
     * Get the column index of a given edge and for a particular commodity
     */
    int getColumnIndex(const Edge::Ptr& e, uint32_t commodity = 0);

};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_MULTI_COMMODITY_MIN_COST_FLOW_HPP
