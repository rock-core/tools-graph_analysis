#include "MultiCommodityMinCostFlow.hpp"
#include <algorithm>
#include <iterator>
#include <math.h>
#include <boost/filesystem.hpp>
#include <base-logging/Logging.hpp>
#include "../DirectedGraphInterface.hpp"

namespace graph_analysis {
namespace algorithms {

MultiCommodityMinCostFlow::MultiCommodityMinCostFlow(const BaseGraph::Ptr& graph, uint32_t commodities, LPSolver::Type solverType)
    : GLPKSolver("multicommodity_min_cost_flow")
    , mpSolver( LPSolver::getInstance(solverType) )
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

    // Pick any available edge to retrieve the number of commodities
    uint32_t edgeAssociatedCommodity = 0;
    EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
    while(edgeIt->next())
    {
        MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(edgeIt->current());
        edgeAssociatedCommodity = edge->numberOfCommodities();
        break;
    }

    if(commodities == 0)
    {
        mCommodities = edgeAssociatedCommodity;
        LOG_INFO_S << "Number of commodities inferred from edges: " << mCommodities;
    }

    // We assume that the edges are always containing the correct information
    // about commodities
    assert(mCommodities == edgeAssociatedCommodity);
}


MultiCommodityMinCostFlow MultiCommodityMinCostFlow::fromFile(const std::string& filename,
        representation::Type format,
        LPSolver::Type solverType)
{
    BaseGraph::Ptr graph = BaseGraph::getInstance();
    io::GraphIO::read(filename, graph, format);

    // In order to infer number of commodities, set to 0
    uint32_t commodities = 0;
    return MultiCommodityMinCostFlow(graph,commodities, solverType);
}

void MultiCommodityMinCostFlow::save(const std::string& filename, representation::Type format)
{
    io::GraphIO::write(filename, mpGraph, format);
}

LPSolver::Status MultiCommodityMinCostFlow::solve(const std::string& prefix, bool useCaching)
{

    LPSolver::Status status;
    switch(mpSolver->getSolverType())
    {
        case GLPK_SOLVER:
            {
                std::string problemFile = createProblem(GLPK);
                status = mpSolver->run(problemFile, GLPK, useCaching);
            }
            break;
        case SCIP_SOLVER:
            {
                std::string problemFile = createProblem(CPLEX);
                status = mpSolver->run(problemFile, CPLEX, useCaching);
            }
            break;
        default:
            throw std::invalid_argument("graph_analysis::algorithms::MultiCommodityMinCostFlow::solve: solver type not supported: " + LPSolver::TypeTxt[mpSolver->getSolverType()] );
            break;
    }

    std::string filename;
    if(prefix.empty())
    {
        filename = mpSolver->saveSolutionToTempfile(BASIC_SOLUTION);
    } else {
        filename = prefix + ".problem";
        mpSolver->saveProblem(filename);

        filename = prefix + ".solution";
        mpSolver->saveSolution(filename, BASIC_SOLUTION);
    }

    storeResult();
    return status;
}

double MultiCommodityMinCostFlow::getCost() const
{
    return mpSolver->getObjectiveValue();
}

std::string MultiCommodityMinCostFlow::createProblem(LPSolver::ProblemFormat format)
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
    // e.g. for
    //     maximize
    //          z = 10x_1 + 6x_2 + 4x3
    //
    //      subject to
    //          p =   x_1 +  x_2 +  x_3
    //          q = 10x_1 + 4x_2 + 5x_3
    //          r =  2x_1 + 2x_2 + 6x_3
    //
    //      and bounds of variables
    //          -inf < p <= 100    0 <= x_1 < +inf
    //          -inf < q <= 600    0 <= x_2 < +inf
    //          -inf < r <= 300    0 <= x_3 < +inf
    //
    // e.g.,
    // describing the first rows coefficients
    //  ia[1]=1, ja[1]=1, ar[1] = 1.0
    //  ia[2]=1, ja[2]=2, ar[2] = 1.0
    //  ia[3]=1, ja[3]=3, ar[3] = 1.0
    //
    // describing the second rows coefficients
    //  ia[4]=2, ja[4]=1, ar[4] = 10.0
    //  ia[5]=2, ja[5]=2, ar[5] = 4.0
    //  ia[7]=2, ja[7]=3, ar[2] = 5.0
    //
    // ia: row indeces
    int ia[1 + numberOfIndices];
    // ja: column indeces
    int ja[1 + numberOfIndices];
    // numerical values of elements corresponding to ia and ja values
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
        uint32_t capacityUpperBound = edge->getCapacityUpperBound();
        if(capacityUpperBound == 0)
        {
            throw
                std::runtime_error("graph_analysis::algorithms::MultiCommodityMinCostFlow::run:"
                        " trying to add edge with CapacityUpperBound = 0");
        } else {
            glp_set_row_bnds(mpProblem, row, GLP_DB, 0.0, capacityUpperBound);
        }

        // Bounds on individual commodity capacities
        for(size_t k = 0; k < mCommodities; ++k,++col,++index)
        {
            glp_add_cols(mpProblem, 1);
            std::string cs = mpSolver->getVariableNameByColumnIdx(col);

            // for each edge create k columns, where k is the number of commodities
            glp_set_col_name(mpProblem, col, cs.c_str());
            // set the bound for the column to 0 as lower and commodity capacity upper bound
            uint32_t commodityCapacityUpperBound = edge->getCommodityCapacityUpperBound(k);
            if(commodityCapacityUpperBound == 0)
            {
                glp_set_col_bnds(mpProblem, col, GLP_FX, 0.0, 0.0);
            } else {
                glp_set_col_bnds(mpProblem, col, GLP_DB, 0.0, commodityCapacityUpperBound);
            }

            LOG_DEBUG_S << "Adding column '" << cs << "' for edge: '" << edge->toString() << "' (id: " << mpGraph->getEdgeId(edge) << ") and commodity '" << k  << "' -- lb: 0.0, ub: " << edge->getCommodityCapacityUpperBound(k);

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
            //
            // Supply: a positive value is supply, negative demand
            int32_t supply = vertex->getCommoditySupply(k);
            glp_set_row_bnds(mpProblem, row, GLP_FX, supply, supply);

            LOG_DEBUG_S << "Adding row '" << rs.str() << "' for vertex '" << mpGraph->getVertexId(vertex) << "' and commodity '" << k << "' with supply: " << supply;
            ++row;

        }

        for(size_t k = 0; k < mCommodities; ++k)
        {

            // Adding all incoming edges and setting the inflow for the
            // associated commodities
            EdgeIterator::Ptr inEdgeIt = diGraph->getInEdgeIterator(vertex);
            while(inEdgeIt->next())
            {
                MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(inEdgeIt->current());
                uint32_t commodityCol = getColumnIndex(edge, k);

                ia[index] = row - mCommodities + k;
                ja[index] = commodityCol;
                // inflow (thus multiply by -1.0)
                ar[index] = -1.0;
                LOG_DEBUG_S << "Add inflow (from incoming edge: " << mpGraph->getEdgeId(edge) << ")" << std::endl
                        << "ij["<< index << "] = " << row - mCommodities + k << std::endl
                        << "ja["<< index << "] = " << commodityCol << std::endl
                        << "ar["<< index << "] = -1.0";
                ++index;
            }

            // Adding all outgoing edges and setting the outflow for the
            // associated commodities
            EdgeIterator::Ptr outEdgeIt = diGraph->getOutEdgeIterator(vertex);
            while(outEdgeIt->next())
            {
                MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>( outEdgeIt->current() );
                uint32_t commodityCol = getColumnIndex(edge, k);

                ia[index] = row - mCommodities + k;
                ja[index] = commodityCol;
                // outflow (thus multiply by 1.0)
                ar[index] = 1.0;

                LOG_DEBUG_S << "Add outflow (from outgoing edge: " << mpGraph->getEdgeId(edge) << ")" << std::endl
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
                    LOG_DEBUG_S << "Add inflow to consider transflow (incoming edge: " << mpGraph->getEdgeId(edge) << ")" << std::endl
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

                    LOG_DEBUG_S << "Add outflow to consider transflow (outgoing edge: " << mpGraph->getEdgeId(edge) << ")" << std::endl
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

    return saveProblemToTempfile(format);
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
            double flow = mpSolver->getVariableValueByColumnIdx(col);


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

        int32_t deltaTransflow = 0;
        int32_t deltaInflow = 0;
        std::set<uint32_t> affectedCommoditiesTransFlow;
        std::set<uint32_t> affectedCommoditiesMinFlow;
        for(uint32_t k = 0; k < mCommodities; ++k)
        {
            int32_t supplyDemand = vertex->getCommoditySupply(k);
            if(supplyDemand < 0) // edge demand
            {
                int32_t delta = supplyDemand + inFlow[k];
                if(delta < 0)
                {
                    constraintViolations.push_back( ConstraintViolation(vertex, k, delta, ConstraintViolation::MinFlow) );
                    deltaInflow += delta;
                    affectedCommoditiesMinFlow.insert(k);
                } else if(delta > 0)
                {
                    // supply is higher than demand
                } else {
                    // demand is perfectly matched
                }
            }

            uint32_t minTransflow = vertex->getCommodityMinTransFlow(k);
            if(minTransflow == 0)
            {
                continue;
            } else if(inFlow[k] < minTransflow || outFlow[k] < minTransflow)
            {
                int32_t delta = std::min(outFlow[k],inFlow[k]) - minTransflow;
                constraintViolations.push_back( ConstraintViolation(vertex, k, delta, ConstraintViolation::TransFlow) );
                deltaTransflow += delta;
                affectedCommoditiesTransFlow.insert(k);
            }
        }

        if(deltaInflow != 0)
        {
            constraintViolations.push_back(ConstraintViolation(vertex, affectedCommoditiesMinFlow, deltaInflow, ConstraintViolation::TotalMinFlow));
        }
        if(deltaTransflow != 0)
        {
            constraintViolations.push_back(ConstraintViolation(vertex, affectedCommoditiesTransFlow, deltaTransflow, ConstraintViolation::TotalTransFlow));
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
