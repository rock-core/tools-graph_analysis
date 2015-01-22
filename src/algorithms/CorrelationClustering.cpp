#include "CorrelationClustering.hpp"

namespace graph_analysis {
namespace algorithms {

void CorrelationClustering::prepare()
{
    // define the integer programm
    mpProblem = glp_create_prob();
    glp_set_prob_name(mpProblem, "correlation_clustering");

    // Set the optimization direction: GLP_MAX -> maximization, GLP_MIN -> minimization
    glp_set_obj_dir(mpProblem, GLP_MIN);

    // Defining the general optimization mpProblem as
    //
    // w = SUM_{E-}(c_e(1-x_e)) + SUM_{E+}(c_e*x_e)
    // allow us to reduce the optimization problem to
    // w = SUM_{E-}c_e - SUM_{E-}(c_e*x_e) + SUM_{E+}(c_e*x_e)
    // and minimizing
    // w = - SUM_{E-}(c_e*x_e) + SUM_{E+}(c_e*x_e)
    // and considering that negative weights belong to E- and positive to E+, we
    // can minimize the following function:
    // w = SUM_{E-}(c_e*x_e) + SUM_{E+}(c_e*x_e}) = SUM_{E}(c_e*x_e)

    // GLP_DB: double bounded variable
    //double sumOfNegativeWeights = 0;
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    size_t col = 1;
    while(edgeIt->next())
    {
        glp_add_cols(mpProblem, 1);
        std::stringstream ss;
        ss << "x" << col;
        glp_set_col_name(mpProblem, col, ss.str().c_str());
        glp_set_col_bnds(mpProblem, col, GLP_DB, 0.0, 1.0);

        // Set the coefficient for this column
        Edge::Ptr edgePtr = edgeIt->current();

        double weight = mEdgeWeightFunction(edgePtr);
        glp_set_obj_coef(mpProblem, col, weight);

        mColumnToEdge.push_back(edgePtr);

        col++;
    }

    mTotalNumberOfColumns = col - 1;

    // Formulate the triangle inequality constraint
    // permutation or combination sufficient !?
    std::vector<size_t> edgeIndices;
    for(size_t i = 0; i < mTotalNumberOfColumns; ++i)
    {
        edgeIndices.push_back(i);
    }

    numeric::Combination<size_t> combination(edgeIndices, 3, numeric::EXACT);
    int ia[1+1000], ja[1+1000];
    double ar[1+1000];

    int i_index = 1;
    int j_index = 1;
    int ar_index = 1;
    do {
        std::vector<size_t> triangle = combination.current();

        numeric::Permutation<size_t> permutation(triangle);
        do {
            std::vector<size_t> triangle_permutation = permutation.current();

            int rowNumber = glp_add_rows(mpProblem, 1);
            std::stringstream ss;
            ss << rowNumber;
            glp_set_row_name(mpProblem, rowNumber, ss.str().c_str());

            /// triangle inequality for three edges
            // x12 + x23 >= x13
            // 0 >= 1*x13 + (-1)*x12 + (-1)*x23 + 0*xREST
            // GLP_UP for upper bound, last argument is the upper bound
            glp_set_row_bnds(mpProblem, rowNumber, GLP_UP, 0.0, 0.0);

            for(size_t i = 0; i < 3; ++i)
            {
                ia[i_index++] = rowNumber;
                ja[j_index++] = triangle_permutation[i] + 1;
                if(i == 0)
                {
                    ar[ar_index++] = 1.0;
                } else {
                    ar[ar_index++] = -1.0;
                }
            }
        } while(permutation.next());
    } while(combination.next());

    glp_load_matrix(mpProblem, i_index - 1, ia, ja, ar);
}

void CorrelationClustering::solve()
{
    // Run simplex algorithm to solve the given optimization problem
    glp_simplex(mpProblem, NULL);

    for(size_t i = 1; i <= mTotalNumberOfColumns; ++i)
    {
        // Retrieve solutions by column index
        double activation = glp_get_col_prim(mpProblem, i);

        Edge::Ptr edge = mColumnToEdge[i-1];
        mEdgeActivation[edge] = activation;
    }

    glp_delete_prob(mpProblem);
}

void CorrelationClustering::round()
{
    // Algorithm ROUND as described by Demaine et al.
    // 1. pick any node in g
    // 2. initialize r to 0
    // 3. grow r by min{(d_uv - r) > 0: v not in B(u,r)} so that B(u,r) includes another entire edge, and repeat until cut(B(u,r)) >= c ln(n+1) x vol(B(u,r))
    // 4. output the vertices in B(u,r) as one of the clusters in S
    // 5. remove vertices in B(u,r) (and incident edges from G
    // 6. Repeat step 1-5 until G is empty

    while(true)
    {
        VertexIterator::Ptr vit = mpGraph->getVertexIterator();
        double radius = 0;

        if(vit->next())
        {
            Vertex::Ptr vertex = vit->current();

        } else {
            // graph is empty
            break;
        }
    }
}

CorrelationClustering::CorrelationClustering(BaseGraph* graph, EdgeWeightFunction weightfunction)
    : mpGraph(graph)
    , mEdgeWeightFunction(weightfunction)
    , mpProblem(0)
{
    prepare();
    solve();
}

std::string CorrelationClustering::toString(const std::map<Edge::Ptr, double>& solution)
{
    std::stringstream ss;
    std::map<Edge::Ptr, double>::const_iterator it = solution.begin();
    for(; it != solution.end(); ++it)
    {
        Edge::Ptr e = it->first;
        ss << e->toString() << " -- " << it->second << std::endl;
    }
    return ss.str();
}

} // end namespace algorithms
} // end namespace graph_analysis
