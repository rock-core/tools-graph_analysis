#include "CorrelationClustering.hpp"
#include <base/Logging.hpp>

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
    // since SUM_{E-}c_e remains constant
    // and considering that negative weights belong to E- and positive to E+, we
    // can minimize the following function:
    // w = SUM_{E-}(c_e*x_e) + SUM_{E+}(c_e*x_e}) = SUM_{E}(c_e*x_e)

    // GLP_DB: double bounded variable
    //double sumOfNegativeWeights = 0;
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    size_t col = 1;
    while(edgeIt->next())
    {
        // for each edge create a column
        glp_add_cols(mpProblem, 1);
        std::stringstream ss;
        ss << "x" << col;
        glp_set_col_name(mpProblem, col, ss.str().c_str());
        // set the bound for the column to 0 and 1 (since this is actually an
        // integer linear programming problem)
        glp_set_col_bnds(mpProblem, col, GLP_DB, 0.0, 1.0);

        // Set the coefficient for this column, i.e. the weight
        Edge::Ptr edgePtr = edgeIt->current();

        double weight = mEdgeWeightFunction(edgePtr);
        glp_set_obj_coef(mpProblem, col, weight);

        mColumnToEdge.push_back(edgePtr);

        col++;
    }

    mTotalNumberOfColumns = col - 1;

    // Formulate the triangle inequality constraint
    std::vector<size_t> edgeIndices;
    for(size_t i = 0; i < mTotalNumberOfColumns; ++i)
    {
        edgeIndices.push_back(i);
    }

    numeric::Combination<size_t> combination(edgeIndices, 3, numeric::EXACT);

    // triangle count: 3! = 6
    int maxRowCount = combination.numberOfCombinations()*6;
    int maxColumnCount = 3;

    // List of row indexes
    int ia[1+maxRowCount*maxColumnCount];
    // List of column indexes
    int ja[1+maxRowCount*maxColumnCount];
    // Numeric values of the corresponding constraint coefficient (ia[i],ja[i]) -> ar[i]
    double ar[1+maxRowCount*maxColumnCount];

    int i_index = 1;
    int j_index = 1;
    int ar_index = 1;
    do {
        // Select the a triangle
        std::vector<size_t> triangle = combination.current();

        // Generate the constraints for this triangle
        numeric::Permutation<size_t> permutation(triangle);
        do {
            // the permutation of indexes pointing to an edge (by
            // column index)
            std::vector<size_t> triangle_permutation = permutation.current();

            // Add a row to define the triangle constraint
            int rowNumber = glp_add_rows(mpProblem, 1);
            std::stringstream ss;
            ss << rowNumber;
            glp_set_row_name(mpProblem, rowNumber, ss.str().c_str());

            /// triangle inequality for three edges
            // x12 + x23 >= x13
            // 0 >= 1*x13 + (-1)*x12 + (-1)*x23 + 0*xREST
            // GLP_UP for upper bound, last argument is the upper bound
            glp_set_row_bnds(mpProblem, rowNumber, GLP_UP, 0.0, 0.0);

            // set factors for x13, x12 and x23
            for(size_t i = 0; i < 3; ++i)
            {
                ia[i_index++] = rowNumber;
                ja[j_index++] = triangle_permutation[i] + 1 /* columns starting at 1*/;
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

double CorrelationClustering::cut(Ball ball)
{
    LOG_INFO_S << "Compute cut";
    double sumOfCutPositiveEdges = 0;
    // Find the edges that have only a single node in ball
    VertexIterator::Ptr nodeIt = ball.graph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr currentVertex = nodeIt->current();

        {
            EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(currentVertex);
            while(edgeIt->next())
            {
                Edge::Ptr edge = edgeIt->current();

                // Check if this is a cut edge
                if(!ball.graph->contains(edge->getSourceVertex()) || !ball.graph->contains(edge->getTargetVertex()))
                {
                    // Check if this a positive edge
                    double edgeWeight = mEdgeWeightFunction(edge);
                    if(edgeWeight > 0)
                    {
                         sumOfCutPositiveEdges += edgeWeight;
                    }
                }
            }
        }
    }

    LOG_INFO_S << "Computed cut: " << sumOfCutPositiveEdges;
    return sumOfCutPositiveEdges;
}

double CorrelationClustering::volume(Ball ball)
{
    LOG_INFO_S << "Compute volume";
    if(ball.radius == 0)
    {
        LOG_INFO_S << "Return initial volume: " << mInitialVolume;
        return mInitialVolume;
    }

    double sumOfCutPositiveEdges = 0;
    // Find the edges that have only a single node in ball
    VertexIterator::Ptr nodeIt = ball.graph->getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr currentVertex = nodeIt->current();

        EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(currentVertex);
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            // Check if this a positive edge
            double edgeWeight = mEdgeWeightFunction(edge);
            if(edgeWeight > 0)
            {
                // Check if this is a cut edge
                if(!ball.graph->contains(edge->getSourceVertex()) || !ball.graph->contains(edge->getTargetVertex()))
                {
                    sumOfCutPositiveEdges += edgeWeight*(ball.radius - mEdgeActivation[edge]);
                } else {
                    sumOfCutPositiveEdges += edgeWeight;
                }
            }
        }
    }
    LOG_INFO_S << "Computed volume: " << sumOfCutPositiveEdges;
    return sumOfCutPositiveEdges;
}

Ball CorrelationClustering::createBall(Vertex::Ptr vertex, double radius)
{
    Ball ball;
    ball.graph = mpGraph->newInstance();
    ball.radius = radius;

    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator(vertex);
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        double activation = mEdgeActivation[edge];
        if(activation <= radius)
        {
            ball.graph->addEdge(edge);
        }
    }

    return ball;
}

    //bool init = false;
    //double bestWeight;
    //Edge::Ptr bestEdge;
    //{
    //    // Iterate over all edges of a vertex
    //    EdgeIterator::Ptr eit = graph->getEdgeIterator(vertex);
    //    while(eit->next())
    //    {
    //        Edge::Ptr edge = eit->current();
    //        if(!ball.contains(edge))
    //        {
    //            double weight = mEdgeWeightFunction(edge);
    //            if(!init || bestWeight > weight)
    //            {
    //                init = true;
    //                bestWeight = weight;
    //                bestEdge = edge;
    //            }
    //        }
    //    }
    //}

        //    // select edge whith smallest positive weight --> add to ball
        //    // repeat until
        //    // cut(ball) => 2.01*log(numberOfNodes +1) * vol(ball)
        //
        //    // mark all vertices in ball as one of the clusters in S, i.e.
        //    // --> mark edges to be 0 within this cluster
        //    //
        //    // remove ball from graph


void CorrelationClustering::round()
{
    // c_uw --> edge weight between vertices u and w
    // x_uw --> a zero-one variable (0 if u and w are in common cluster, 1
    // otherwise)
    // Algorithm ROUND as described by Demaine et al.
    // 1. pick any node in g
    // 2. initialize r to 0
    // 3. grow r by min{(d_uv - r) > 0: v not in B(u,r)} so that B(u,r) includes another entire edge, and repeat until cut(B(u,r)) <= c ln(n+1) x vol(B(u,r))
    //    cut(B(u,r)) <= c ln(n+1) x vol(B(u,r)) provided r <= 1/c  and
    //    eventually c > 2 "c, is just slightly larger than 2"
    // 4. output the vertices in B(u,r) as one of the clusters in S
    // 5. remove vertices in B(u,r) (and incident edges from G
    // 6. Repeat step 1-5 until G is empty
    //
    // ball(node, r) --> defines nodes and edges (subgraph) of all nodes v such that x_uv <= r,
    // subgraph induced by
    // these nodes, and the fraction (r -x_uv)/x_vw of edges(v,w) with only
    // endpoint v \in B(u,r) -- meaning otherwise they form their own cluster?!
    //
    // cut(S) where S is a set of nodes --> weight of positive edges with
    // exactly one endpoint in S
    //      cut(S) = \sum_{ {v,w} \supset S, (v,w) \in E^{+} c_{vw}
    //
    // cut(ball) is cut induced by the set of vertices included in the ball
    //
    // vol(S) weighted distance of the edges with both endpoints in S
    //      vol(S) = \sum_{ {v,w} \supset S, (v,w) \in E^{+} c_{vw}x_{vw}
    //
    // vol(ball) volume of a ball volume of nodes in B including the fractional
    // weighted distance of positive edes leaving B(u,r)
    //     if(v,w) \in E^{+} is a cut positive edge of ball B(u,r)
    //         with v \in B(u,r) and w \not\in B(u,r)
    //         --> + c_{vw}*(r-x_{uv})

    // A graph which can be manipulated
    LOG_INFO_S << "Create copy";
    BaseGraph::Ptr graph = mpGraph->copy();

    LOG_INFO_S << "Setup ball";
    while(true)
    {
        LOG_INFO_S << "Pick vertex";
        VertexIterator::Ptr vit = graph->getVertexIterator();

        if(vit->next())
        {
            // 1. Pick a, i.e. the first node
            Vertex::Ptr vertex = vit->current();
            // 2. Set radius to 0 and initialize the ball
            Ball ball;
            ball.graph = graph->newInstance();
            ball.graph->addVertex(vertex);
            ball.radius = 0;

            // 3. Grow the ball by another entire edge
            LOG_INFO_S << "Grow ball";
            while( cut(ball) >= mConstant*volume(ball) )
            {
                double minDistance = std::numeric_limits<double>::infinity();
                Edge::Ptr minEdge;

                // find minimal distance for vertex to next vertex
                // and increase radius
                LOG_INFO_S << "Get edge iterator";
                EdgeIterator::Ptr edgeIt = graph->getEdgeIterator(vertex);
                while(edgeIt->next())
                {
                    Edge::Ptr edge = edgeIt->current();
                    LOG_INFO_S << "CurrentEdge: " << edge->toString();
                    // Check if the other end of the edge lies outside of the
                    // ball
                    LOG_INFO_S << "Check edge in ball";
                    if(!ball.graph)
                    {
                        LOG_WARN_S << "Ball has no graph";
                    }
                    if(!ball.graph->contains(edge->getSourceVertex()) || !ball.graph->contains(edge->getTargetVertex()))
                    {
                        double currentDistance = mEdgeActivation[edge];
                        LOG_INFO_S << "Check current distance: " << currentDistance;
                        if(currentDistance < ball.radius)
                        {
                            throw std::runtime_error("This edge should be already included in the ball");
                        } else {
                            if(currentDistance > 0 && currentDistance < minDistance)
                            {
                                LOG_INFO_S << "Update minDistance: " << currentDistance;
                                minDistance = currentDistance;
                                minEdge = edge;
                            }
                        }
                    }
                }

                LOG_INFO_S << "Current min distance: " << minDistance;
                ball.radius = minDistance;
                if(minEdge)
                {
                    ball.graph->addEdge(minEdge);
                } else {
                    LOG_INFO_S << "Ball no edge left to add";
                    break;
                }
            }

            LOG_INFO_S << "Marking cluster";

            // 4. Mark ball as in the same cluster
            EdgeIterator::Ptr edgeIt = ball.graph->getEdgeIterator();
            while(edgeIt->next())
            {
                Edge::Ptr edge = edgeIt->current();
                // update the edge activation
                LOG_DEBUG_S << "Update edge association: was << " << mEdgeActivation[edge] << " now set to 0";
                mEdgeActivation[edge] = 0;
                graph->removeEdge(edge);
            }
            VertexIterator::Ptr vertexIt = ball.graph->getVertexIterator();
            while(vertexIt->next())
            {
                Vertex::Ptr vertex = vertexIt->current();
                graph->removeVertex(vertex);
            }
            LOG_INFO_S << "Iteration done: " << graph->getVertexCount() << " nodes left";
        } else {
            LOG_INFO_S << "Graph is empty -- algorithm done";
            // graph is empty
            break;
        }
    }
}

CorrelationClustering::CorrelationClustering(BaseGraph::Ptr graph, EdgeWeightFunction weightfunction)
    : mpGraph(graph)
    , mEdgeWeightFunction(weightfunction)
    , mpProblem(0)
{
    Ball ball;
    ball.graph = mpGraph;
    // Prevent premature use of radius in volume(S)
    ball.radius = 1;

    uint64_t nodeCount = graph->getVertexCount();
    mConstant = 2.01* log(nodeCount + 1);
    mInitialVolume = volume(ball) / (nodeCount*1.0);

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
