#include "MultiCommodityMinCostFlow.hpp"
#include <algorithm>
#include <iterator>
#include <math.h>
#include <boost/filesystem.hpp>
#include <base-logging/Logging.hpp>
#include <base/Time.hpp>
#include "../DirectedGraphInterface.hpp"
#include "lp/Problem.hpp"

namespace graph_analysis {
namespace algorithms {

MultiCommodityMinCostFlow::MultiCommodityMinCostFlow(const BaseGraph::Ptr& graph, uint32_t commodities, LPSolver::Type solverType)
    : mpSolver( LPSolver::getInstance(solverType) )
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
    std::string problemFile = createProblem(LPSolver::CPLEX);
    status = mpSolver->run(problemFile, LPSolver::CPLEX, useCaching);

    if(problemFile.empty())
    {
        throw std::runtime_error("graph_analysis::algorithms::MultiCommodityMinCostFlow::solve: problem file not available");
    }

    std::string filename;
    if(prefix.empty())
    {
        filename = mpSolver->saveSolutionToTempfile(LPSolver::BASIC_SOLUTION);
    } else {
        filename = prefix + ".problem";
        if(boost::filesystem::exists(filename))
        {
            boost::filesystem::remove(filename);
        }
        boost::filesystem::copy_file(problemFile, filename);

        filename = prefix + ".solution";
        mpSolver->saveSolution(filename, LPSolver::BASIC_SOLUTION);
    }

    storeResult();
    return status;
}

double MultiCommodityMinCostFlow::getCost() const
{
    return mpSolver->getObjectiveValue();
}

std::string MultiCommodityMinCostFlow::createProblemCPLEX()
{
    DirectedGraphInterface::Ptr diGraph = dynamic_pointer_cast<DirectedGraphInterface>(mpGraph);

    std::string problemName = "multicommodity_min_cost_flow-" +
        base::Time::now().toString();

    lp::Problem problem(problemName, lp::OPTIMIZE_MIN );

    size_t col = 1;
    size_t row = 1;

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
        std::stringstream rs;
        rs << "y" << row;

        lp::Row lpRow;
        lpRow.name = rs.str();

        uint32_t capacityUpperBound = edge->getCapacityUpperBound();
        if(capacityUpperBound == 0)
        {
            throw
                std::runtime_error("graph_analysis::algorithms::MultiCommodityMinCostFlow::run:"
                        " trying to add edge with CapacityUpperBound = 0 :"
                        + edge->toString()
                        );
        } else {
            lpRow.bounds = lp::Bounds(0.0, capacityUpperBound, lp::LowerUpper);
        }

        size_t columnBaseLine = 0;
        // Bounds on individual commodity capacities
        for(size_t k = 0; k < mCommodities; ++k,++col)
        {
            std::string cs = mpSolver->getVariableNameByColumnIdx(col);

            // Create column
            // set the bound for the column to 0 as lower and commodity capacity upper bound
            uint32_t commodityCapacityUpperBound = edge->getCommodityCapacityUpperBound(k);

            lp::Column column;
            double commodityCost = edge->getCommodityCost(k);
            if(commodityCapacityUpperBound == 0)
            {
                column = lp::Column(cs, lp::Bounds(0.0, 0.0, lp::Exact), commodityCost );
            } else {
                column = lp::Column(cs, lp::Bounds(0.0, commodityCapacityUpperBound, lp::LowerUpper), commodityCost);
            }

            problem.addColumn(column);
            // Factor the commodities on this edge (always only one)
            lp::MatrixEntry entry(cs, 1.0);
            lpRow.entries.push_back(entry);

            LOG_DEBUG_S << "Adding column '" << cs << "' for edge: '" << edge->toString() << "' (id: " << mpGraph->getEdgeId(edge) << ") and commodity '" << k  << "' -- lb: 0.0, ub: " << edge->getCommodityCapacityUpperBound(k);
        }
        problem.addRow(lpRow);

        // Bounds on combined commodity capacities
        for(const MultiCommodityEdge::SubCapacityUpperBounds::value_type& sub : edge->getSubCapacityBounds())
        {
            std::stringstream rs;
            rs << "y" << ++row;
            uint32_t capacityUpperBound = sub.second;
            // set right hand site bound
            lp::Row subCapacityRow(rs.str(), lp::Bounds(0.0, capacityUpperBound, lp::LowerUpper));

            // define left hand site sum of column value
            for(uint32_t commodity : sub.first)
            {
                std::string cs = mpSolver->getVariableNameByColumnIdx( columnBaseLine + commodity );
                // 'activate' the variable
                lp::MatrixEntry matrixEntry(cs, 1.0);
                subCapacityRow.entries.push_back(matrixEntry);
            }
            LOG_DEBUG_S << "Add subcapacity row: " << subCapacityRow.name;
            problem.addRow(subCapacityRow);
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
        // DEAL WITH INFLOW-OUTFLOW BALANCE PER COMMODITY TYPE
        //---------------------
        // Adding rows for all commodities for a given vertex
        // to set the supply/demand
        for(size_t k = 0; k < mCommodities; ++k)
        {
            std::stringstream rs;
            rs << "y" << row;
            // inflow + demand = 0
            // supply - outflow = 0
            // inflow + (demand + supply) - outflow = 0
            // (demand + supply) = outflow - inflow
            //
            // Supply: a positive value is supply, negative demand
            int32_t supply = vertex->getCommoditySupply(k);

            lp::Row lpRow(rs.str(), lp::Bounds(supply,supply, lp::Exact));
            // Adding all incoming edges and setting the inflow for the
            // associated commodities
            EdgeIterator::Ptr inEdgeIt = diGraph->getInEdgeIterator(vertex);
            while(inEdgeIt->next())
            {
                MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(inEdgeIt->current());
                uint32_t commodityCol = getColumnIndex(edge, k);
                lp::MatrixEntry entry( problem.columnByIdx(commodityCol).name, -1.0);
                lpRow.entries.push_back(entry);
            }

            // Adding all outgoing edges and setting the outflow for the
            // associated commodities
            EdgeIterator::Ptr outEdgeIt = diGraph->getOutEdgeIterator(vertex);
            while(outEdgeIt->next())
            {
                MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>( outEdgeIt->current() );
                uint32_t commodityCol = getColumnIndex(edge, k);

                // outflow (thus multiply by 1.0)
                lp::MatrixEntry entry( problem.columnByIdx(commodityCol).name, 1.0);
                lpRow.entries.push_back(entry);
            }

            if(!lpRow.entries.empty())
            {
                LOG_DEBUG_S << "Adding balance constraint via row '" << rs.str() << "' for vertex '" << mpGraph->getVertexId(vertex) << "' and commodity '" << k << "' with supply: " << supply;
                problem.addRow(lpRow);
                mRowToVertexCommodity.push_back(std::pair<Vertex::Ptr,size_t>(vertex, k) );
                ++row;
            }


            //---------------------
            // INFLOW-OUTFLOW MINIMUM LEVEL
            //---------------------
            uint32_t minTransFlow = vertex->getCommodityMinTransFlow(k);
            uint32_t maxTransFlow = vertex->getCommodityMaxTransFlow(k);

            // Optionally set - only when transflow is bounded
            if(maxTransFlow != std::numeric_limits<uint32_t>::max()
                    || minTransFlow != 0)
            {
                // Adding rows for all commodities for a given vertex
                // to set the supply/demand
                std::stringstream rs;
                rs << "y" << row;

                lp::Row lpRow;
                if(maxTransFlow != std::numeric_limits<uint32_t>::max())
                {
                    lpRow = lp::Row(rs.str(), lp::Bounds(minTransFlow, maxTransFlow, lp::LowerUpper));
                } else {
                    lpRow = lp::Row(rs.str(), lp::Bounds(minTransFlow, 0.0, lp::Lower));
                }

                EdgeIterator::Ptr inEdgeIt = diGraph->getInEdgeIterator(vertex);
                while(inEdgeIt->next())
                {
                    MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(inEdgeIt->current());
                    uint32_t commodityCol = getColumnIndex(edge, k);
                    lp::MatrixEntry entry( problem.columnByIdx(commodityCol).name, 1.0);
                    lpRow.entries.push_back(entry);
                }

                if(!lpRow.entries.empty())
                {
                    problem.addRow(lpRow);
                    mRowToVertexCommodity.push_back(std::pair<Vertex::Ptr,size_t>(vertex, k) );
                    LOG_INFO_S << "Add transflow row: " << lpRow.name
                        << " min:" << minTransFlow << " max: " << maxTransFlow
                        << std::endl;
                    ++row;
                }
            } // transflow
        } // end commodities

        //--------------------------
        // MIN/MAX INFLOW
        //--------------------------
        const MultiCommodityVertex::CombinedFlowBounds& minMaxCombinedInFlow = vertex->getCommoditiesInFlowBounds();
        for(const MultiCommodityVertex::CombinedFlowBounds::value_type& combinedBounds : minMaxCombinedInFlow)
        {
            std::stringstream rs;
            rs << "y" << row;
            const MultiCommodityEdge::CommoditySet& commodities = combinedBounds.first;
            std::pair<uint32_t, uint32_t> minMax = combinedBounds.second;

            if( minMax.second != std::numeric_limits<uint32_t>::max()
                    && minMax.first != 0)
            {
                lp::Row lpRow;
                if( minMax.second != std::numeric_limits<uint32_t>::max())
                {
                    lpRow = lp::Row(rs.str(), lp::Bounds(minMax.first, minMax.second, lp::LowerUpper));
                } else {
                    lpRow = lp::Row(rs.str(), lp::Bounds(minMax.first, 0.0, lp::Lower));
                }

                EdgeIterator::Ptr inEdgeIt = diGraph->getInEdgeIterator(vertex);
                while(inEdgeIt->next())
                {
                    MultiCommodityEdge::Ptr edge = dynamic_pointer_cast<MultiCommodityEdge>(inEdgeIt->current());
                    for(size_t k : commodities)
                    {
                        uint32_t commodityCol = getColumnIndex(edge,k);
                        lp::MatrixEntry entry(problem.columnByIdx(commodityCol).name, 1.0);
                        lpRow.entries.push_back(entry);
                    }
                }
                if(!lpRow.entries.empty())
                {
                    problem.addRow(lpRow);
                    LOG_DEBUG_S << "Add min/max inflow row: " << row << lpRow.name << std::endl
                        << " commodities" << commodities.size() << std::endl
                        << " min:" << minMax.first << " max: " << minMax.second
                        << std::endl;
                    ++row;
                } else {
                    LOG_WARN_S << "Add min/max inflow not possible - no inedges";
                }
            }
        }
    }
    mTotalNumberOfColumns = col - 1;
    mTotalNumberOfRows = row - 1;

    return problem.saveProblemToTempfile();
}

std::string MultiCommodityMinCostFlow::createProblem(LPSolver::ProblemFormat format)
{
    switch(format)
    {
        case LPSolver::CPLEX:
            return createProblemCPLEX();
        default:
            throw
                std::invalid_argument("graph_analysis::algorithms::MultiCommodityMinCostFlow:"
                        "format '" + LPSolver::ProblemFormatTxt[format] + "' is"
                        " unsupported for problem generation");
    }
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

MultiCommodityMinCostFlow::VertexCommoditySet MultiCommodityMinCostFlow::vertexConstraints() const
{
    return vertexConstraints( mpSolver->infeasibleConstraints() );
}

MultiCommodityMinCostFlow::VertexCommoditySet MultiCommodityMinCostFlow::vertexConstraints(const std::vector<size_t>& constraintIds) const
{
    VertexCommoditySet vertices;
    size_t edgeConstraints = mColumnToEdge.size();
    for(size_t idx : constraintIds)
    {
        if(idx >= edgeConstraints)
        {
            size_t constraintIdx = idx - edgeConstraints;
            try {
                vertices.insert( mRowToVertexCommodity.at(constraintIdx) );
            } catch(...)
            {
                throw std::invalid_argument("graph_analysis::algorithms::MultiCommodityMinCostFlow::vertexConstraints: vertex idx exceeds number of existing constraints");
            }
        }
    }
    return vertices;
}

std::set<Edge::Ptr> MultiCommodityMinCostFlow::edgeConstraints() const
{
    return edgeConstraints( mpSolver->infeasibleConstraints() );
}

std::set<Edge::Ptr> MultiCommodityMinCostFlow::edgeConstraints(const std::vector<size_t>& constraintIds) const
{
    std::set<Edge::Ptr> edges;
    size_t edgeConstraints = mColumnToEdge.size();
    for(size_t idx : constraintIds)
    {
        if(idx < edgeConstraints)
        {
            try {
                edges.insert(mColumnToEdge.at(idx));
            } catch(...)
            {
                throw std::invalid_argument("graph_analysis::algorithms::MultiCommodityMinCostFlow::edgeConstraints: edge idx exceeds number of existing constraints");
            }
        }
    }
    return edges;
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
        uint32_t totalInFlow = 0;
        uint32_t totalOutFlow = 0;
        std::set<uint32_t> affectedCommoditiesTransFlow;
        std::set<uint32_t> affectedCommoditiesMinFlow;
        for(uint32_t k = 0; k < mCommodities; ++k)
        {
            int32_t supplyDemand = vertex->getCommoditySupply(k);
            totalInFlow += inFlow[k];
            totalOutFlow += outFlow[k];

            if(supplyDemand < 0) // demand
            {
                int32_t delta = supplyDemand + inFlow[k];
                if(delta < 0)
                {
                    constraintViolations.push_back( ConstraintViolation(vertex, k, delta, inFlow[k], outFlow[k], ConstraintViolation::MinFlow) );
                    deltaInflow += delta;
                    affectedCommoditiesMinFlow.insert(k);
                } else if(delta > 0)
                {
                    // supply is higher than demand
                } else {
                    // demand is perfectly matched
                }
            }

            // out = totalInFlow + supply
            int32_t balance = outFlow[k] - inFlow[k] - supplyDemand;
            if(balance != 0)
            {
                constraintViolations.push_back(ConstraintViolation(vertex, k,
                            balance, inFlow[k], outFlow[k],
                            ConstraintViolation::FlowBalance) );
            }

            LOG_INFO_S << "Flow : " << std::endl
                << "    commodity: " << k << std::endl
                << "    in: " << inFlow[k] << std::endl
                << "    out:" << outFlow[k] << std::endl
                << "    supplyDemand: " << supplyDemand << std::endl
                << "    balance: " << balance << std::endl
                << "    vertex: " << vertex->toString() << std::endl;

            uint32_t minTransflow = vertex->getCommodityMinTransFlow(k);
            if(minTransflow == 0)
            {
                continue;
            } else if(inFlow[k] < minTransflow || outFlow[k] < minTransflow)
            {
                int32_t delta = std::min(outFlow[k],inFlow[k]) - minTransflow;
                constraintViolations.push_back( ConstraintViolation(vertex, k, delta, inFlow[k], outFlow[k], ConstraintViolation::TransFlow) );
                deltaTransflow += delta;
                affectedCommoditiesTransFlow.insert(k);
            }

        }

        if(deltaInflow != 0)
        {
            constraintViolations.push_back(ConstraintViolation(vertex, affectedCommoditiesMinFlow, deltaInflow, totalInFlow, totalOutFlow, ConstraintViolation::TotalMinFlow));
        }
        if(deltaTransflow != 0)
        {
            constraintViolations.push_back(ConstraintViolation(vertex, affectedCommoditiesTransFlow, deltaTransflow, totalInFlow, totalOutFlow, ConstraintViolation::TotalTransFlow));
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
