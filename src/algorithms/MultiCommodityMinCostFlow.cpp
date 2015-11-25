#include "MultiCommodityMinCostFlow.hpp"
#include <algorithm>
#include <iterator>
#include <math.h>
#include <graph_analysis/DirectedGraphInterface.hpp>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace algorithms {

MultiCommodityMinCostFlow::MultiCommodityMinCostFlow(const BaseGraph::Ptr& graph, uint32_t commodities)
    : GLPKSolver("multicommodity_min_cost_flow")
    , mCommodities(commodities)
    , mpGraph(graph)
    , mTotalNumberOfColumns(0)
    , mTotalNumberOfRows(0)
{
    if(!graph)
    {
        throw std::invalid_argument("graph_analysis::algorithms::MultiCommodityMinCostFlow:"
                " pointer to graph is null");
    }

    if(!dynamic_pointer_cast<DirectedGraphInterface>(mpGraph))
    {
        throw std::invalid_argument("graph_analysis::algorithms::MultiCommodityMinCostFlow:"
                " given graph is not directed (or cannot be casted to DirectedGraphInterface)");
    }
}

GLPKSolver::Status MultiCommodityMinCostFlow::run()
{
    DirectedGraphInterface::Ptr diGraph = dynamic_pointer_cast<DirectedGraphInterface>(mpGraph);

    // foreach edge
    //     consumed capacity per node <= upper bound
    //     consumed capacity per node and commodity <= upper bound (of edge for this commodity)
    //
    //     cost per edge = sum "consumed capacity per commodity"*"cost per commodity"
    //
    // group vertices as (same location) and source,target,transshipment node
    //    --- node: per supply/demand per commodity +,- or null + -> supply, -
    //    -> demand, 0 transshipment
    //
    // foreach vertex
    //     if transshipment node
    //         overall (timewise) inflow = outflow, i.e. consumed capacity overall all incoming edges - consumed capacity over all outgoing edges = 0
    //         set min level of inflow / outflow to 'control/ancor' flow 
    //     flow

    // # orderOfGraph rows to define balance constraints (from in/out flow on a vertex);
    size_t orderOfGraph = mpGraph->order();
    // # sizeOfGraph rows to define edge bound
    size_t sizeOfGraph = mpGraph->size();

    // This is an upper bound for the number of indices
    size_t numberOfIndices = sizeOfGraph*mCommodities + 3*orderOfGraph*sqrt(sizeOfGraph)*mCommodities;
    LOG_DEBUG_S << "Order of graph: " << orderOfGraph << ", sizeOfGraph " << sizeOfGraph << ", commodities: " << mCommodities << " --> NumberOfIndices (upper bound): " << numberOfIndices;
    if(numberOfIndices > 5E06)
    {
        throw std::runtime_error("graph_analysis::algorithms::MultiCommodityMinCostFlow::run: numberOfIndices for formulation"
                " of the problem exceeds 5E06");
    }

    // first index
    int ia[1 + numberOfIndices];
    int ja[1 + numberOfIndices];
    double ar[1 + numberOfIndices];


    // define the integer program
    mpProblem = glp_create_prob();
    glp_set_prob_name(mpProblem, "multicommodity_min_cost_flow");

    // Optimization direction: GLP_MIN
    glp_set_obj_dir(mpProblem, GLP_MIN);

    size_t col = 1;
    size_t row = 1;

    size_t index = 1;

    // columns: e0-k1 e0-k2 e0-k2 e0-k3 ... e1-k1 e1-k2 e1-k3 ...
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        // add 
        // MaxEdgeCapacity >=  0*... + 1.0*currentEdgeComm0 + 1.0*currentEdgeComm1 ... + 0*edgeCom
        MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>( edgeIt->current() );
        if(!edge)
        {
            throw std::runtime_error("graph_analysis::algorithms::MultiCommodityMinCostFlow::run cannot cast edge to MultiCommodityEdge");
        }

        // Start column --> mColumnToEdge*numberOfCommodities + commodityOffset
        // commodityOffset := 1 .. K
        mColumnToEdge.push_back(edge);

        // Bound on total capacity
        glp_add_rows(mpProblem, 1);
        std::stringstream rs;
        rs << "y" << row;
        glp_set_row_name(mpProblem, row, rs.str().c_str());
        glp_set_row_bnds(mpProblem, row, GLP_DB, 0.0, edge->getCapacityUpperBound());

        // Bounds on individual commodity capacities
        for(size_t k = 0; k < mCommodities; ++k,++col,++index)
        {
            glp_add_cols(mpProblem, 1);
            std::stringstream cs;
            cs << "x" << col;
            // for each edge create k columns, where k is the number of commodities 
            glp_set_col_name(mpProblem, col, cs.str().c_str());
            // set the bound for the column to 0 as lower and commodity capacity upper bound
            glp_set_col_bnds(mpProblem, col, GLP_DB, 0.0, edge->getCommodityCapacityUpperBound(k));

            LOG_DEBUG_S << "Adding column '" << cs.str() << "' for edge: '" << edge->toString() << "' and commodity '" << k  << "' -- lb: 0.0, ub: " << edge->getCommodityCapacityUpperBound(k);

            // Set column to GLP_IV := integer variable (see documentationc chapter 2.10.1)
            // (other types are GLP_CV := continuous or GLP_BV := binary variabe)
            glp_set_col_kind(mpProblem, col, GLP_IV);

            // Set the coefficient of the object function to commodity cost
            glp_set_obj_coef(mpProblem, col, edge->getCommodityCost(k));

            ia[index] = row;
            ja[index] = col;
            ar[index] = 1.0;

            LOG_DEBUG_S << "Add" << std::endl
                    << "ij["<< index << "] = " << row << std::endl
                    << "ja["<< index << "] = " << col << std::endl
                    << "ar["<< index << "] = 1.0";
        }
        ++row;
    }
    // no need for grouping even for time expanded networks, since the condition 
    // still holds if the demand is not used, then we transport it locally on
    // that edge -- allow to identify locally positioned items (at higher cost)
    VertexIterator::Ptr vertexIt = mpGraph->getVertexIterator();
    while(vertexIt->next())
    {
        MultiCommodityVertex::Ptr vertex = dynamic_pointer_cast<MultiCommodityVertex>( vertexIt->current() );
        if(!vertex)
        {
            throw std::runtime_error("graph_analysis::algorithms::MultiCommodityMinCostFlow: cannot cast vertex to MultiCommodityVertex");
        }

        //---------------------
        // DEAL WITH INFLOW-OUTFLOW BALANCE
        //---------------------
        // Adding rows for all commodities for a given vertex 
        // to set the supply/demand
        for(size_t k = 0; k < mCommodities; ++k)
        {
            glp_add_rows(mpProblem, 1);
            std::stringstream rs;
            rs << "y" << row;
            glp_set_row_name(mpProblem, row, rs.str().c_str());
            // inflow + demand = 0
            // supply - outflow = 0
            // inflow + (demand + supply) - outflow = 0
            // (demand + supply) = outflow - inflow
            int32_t supply = vertex->getCommoditySupply(k);
            glp_set_row_bnds(mpProblem, row, GLP_FX, supply, supply);

            LOG_DEBUG_S << "Adding row '" << rs.str() << "' for vertex '" << mpGraph->getVertexId(vertex) << "' and commodity '" << k << "' with supply: " << supply;
            ++row;
            
        }

        for(size_t k = 0; k < mCommodities; ++k)
        {
            EdgeIterator::Ptr inEdgeIt = diGraph->getInEdgeIterator(vertex);
            while(inEdgeIt->next())
            {
                MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(inEdgeIt->current());
                uint32_t commodityCol = getColumnIndex(edge, k);

                ia[index] = row - mCommodities + k;
                ja[index] = commodityCol;
                // inflow (thus multiply by -1.0)
                ar[index] = -1.0;
                LOG_DEBUG_S << "Add out edge: " << mpGraph->getEdgeId(edge) << std::endl
                        << "ij["<< index << "] = " << row - mCommodities + k << std::endl
                        << "ja["<< index << "] = " << commodityCol << std::endl
                        << "ar["<< index << "] = -1.0";
                ++index;
            }

            EdgeIterator::Ptr outEdgeIt = diGraph->getOutEdgeIterator(vertex);
            while(outEdgeIt->next())
            {
                MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>( outEdgeIt->current() );
                uint32_t commodityCol = getColumnIndex(edge, k);

                ia[index] = row - mCommodities + k;
                ja[index] = commodityCol;
                // outflow (thus multiply by 1.0)
                ar[index] = 1.0;

                LOG_DEBUG_S << "Add in edge: " << mpGraph->getEdgeId(edge) << std::endl
                        << "ij["<< index << "] = " << row - mCommodities + k << std::endl
                        << "ja["<< index << "] = " << commodityCol << std::endl
                        << "ar["<< index << "] = 1.0";
                ++index;
            }
        }

        //---------------------
        // INFLOW-OUTFLOW MINIMUM LEVEL
        //---------------------
        // Adding rows for all commodities for a given vertex 
        // to set the supply/demand
        for(size_t k = 0; k < mCommodities; ++k)
        {
            if(vertex->getCommoditySupply(k) == 0)
            {
                glp_add_rows(mpProblem, 1);
                {
                    std::stringstream rs;
                    rs << "y" << row;
                    glp_set_row_name(mpProblem, row, rs.str().c_str());
                    uint32_t minTransFlow = vertex->getCommodityMinTransFlow(k);
                    glp_set_row_bnds(mpProblem, row, GLP_LO, minTransFlow, 0.0);

                    LOG_DEBUG_S << "Adding row '" << rs.str() << "' for vertex '" << mpGraph->getVertexId(vertex) << "' and commodity '" << k << "' with min trans flow: " << minTransFlow;
                }


                EdgeIterator::Ptr inEdgeIt = diGraph->getInEdgeIterator(vertex);
                while(inEdgeIt->next())
                {
                    MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(inEdgeIt->current());
                    uint32_t commodityCol = getColumnIndex(edge, k);

                    ia[index] = row;
                    ja[index] = commodityCol;
                    // inflow (thus multiply by -1.0)
                    ar[index] = -1.0;
                    LOG_DEBUG_S << "Add out edge: " << mpGraph->getEdgeId(edge) << std::endl
                            << "ij["<< index << "] = " << row - mCommodities + k << std::endl
                            << "ja["<< index << "] = " << commodityCol << std::endl
                            << "ar["<< index << "] = -1.0";
                    ++index;
                }

                EdgeIterator::Ptr outEdgeIt = diGraph->getOutEdgeIterator(vertex);
                while(outEdgeIt->next())
                {
                    MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>( outEdgeIt->current() );
                    uint32_t commodityCol = getColumnIndex(edge, k);

                    ia[index] = row;
                    ja[index] = commodityCol;
                    // outflow (thus multiply by 1.0)
                    ar[index] = 1.0;

                    LOG_DEBUG_S << "Add in edge: " << mpGraph->getEdgeId(edge) << std::endl
                            << "ij["<< index << "] = " << row - mCommodities + k << std::endl
                            << "ja["<< index << "] = " << commodityCol << std::endl
                            << "ar["<< index << "] = 1.0";
                    ++index;
                }
                ++row;
            }
        }
    }
    mTotalNumberOfColumns = col - 1;
    mTotalNumberOfRows = row - 1;

    LOG_INFO_S << "MultiCommodityMinCostFlow: size of load matrix " << index - 1;
    glp_load_matrix(mpProblem, index - 1, ia, ja, ar);

    // SIMPLEX
    int result = glp_simplex(mpProblem, NULL);
    return translateSimplexReturnCode(result);
}

void MultiCommodityMinCostFlow::storeResult()
{
    // Write solution
    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
    while(edgeIt->next())
    {
        MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>( edgeIt->current() );
        if(!edge)
        {
            throw std::runtime_error("graph_analysis::algorithms::MultiCommodityMinCostFlow::storeResult: "
                    "cannot cast edge to MultiCommodityEdge");
        }

        for(uint32_t k = 0; k < mCommodities; ++k)
        {
            uint32_t col = getColumnIndex(edge, k);
            double flow = glp_get_col_prim(mpProblem, col);

            edge->setCommodityFlow(k, ceil(flow) );

        }
    }
}

std::vector<ConstraintViolation> MultiCommodityMinCostFlow::validateInflow() const
{
    std::vector<ConstraintViolation> constraintViolations;

    DirectedGraphInterface::Ptr diGraph = dynamic_pointer_cast<DirectedGraphInterface>(mpGraph);
    VertexIterator::Ptr vertexIt = diGraph->getVertexIterator();
    while(vertexIt->next())
    {
        MultiCommodityVertex::Ptr vertex = dynamic_pointer_cast<MultiCommodityVertex>(vertexIt->current());

        std::vector<uint32_t> inFlow(mCommodities,0);
        std::vector<uint32_t> outFlow(mCommodities,0);
        for(uint32_t k = 0; k < mCommodities; ++k)
        {
            {
                EdgeIterator::Ptr edgeIt = diGraph->getInEdgeIterator(vertex);
                while(edgeIt->next())
                {
                    MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(edgeIt->current());
                    inFlow[k] += edge->getCommodityFlow(k);
                }
            }
            {
                EdgeIterator::Ptr edgeIt = diGraph->getOutEdgeIterator(vertex);
                while(edgeIt->next())
                {
                    MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(edgeIt->current());
                    outFlow[k] += edge->getCommodityFlow(k);
                }
            }
        }
        for(uint32_t k = 0; k < mCommodities; ++k)
        {
            int32_t supply = vertex->getCommoditySupply(k);
            uint32_t minTransflow = vertex->getCommodityMinTransFlow(k);

            if(supply < 0) // edge demand
            {
                 int32_t delta = supply + inFlow[k];
                 if(delta != 0)
                 {
                     constraintViolations.push_back( ConstraintViolation(vertex, k, delta) );
                 }
            }

            if(minTransflow == 0)
            {
                continue;
            } else if(outFlow[k] < minTransflow)
            {
                constraintViolations.push_back( ConstraintViolation(vertex, k, outFlow[k] - minTransflow, ConstraintViolation::TransFlow) );
            }
        }
    }
    return constraintViolations;
}

int MultiCommodityMinCostFlow::getColumnIndex(const Edge::Ptr& e, uint32_t commodity)
{
    std::vector<Edge::Ptr>::iterator cit = std::find(mColumnToEdge.begin(), mColumnToEdge.end(), e);
    if(cit != mColumnToEdge.end())
    {
        int distance = std::distance(mColumnToEdge.begin(), cit)*mCommodities + commodity + 1;
        LOG_DEBUG_S << "Column index '" << e->toString() << "' commodity: '" << commodity << "' is " << distance;
        return distance; 
    }
    throw std::invalid_argument("graph_analysis::algorithms::MultiCommodityMinCostFlow::getColumnIndex: edge '" + e->toString() + "' is not associated with a column");
}

} // end namespace algorithms
} // end namespace graph_analysis
