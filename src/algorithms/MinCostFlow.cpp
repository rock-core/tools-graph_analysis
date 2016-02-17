#include "MinCostFlow.hpp"

#include <lemon/network_simplex.h>
#include <lemon/cycle_canceling.h>
#include <graph_analysis/lemon/DirectedGraph.hpp>

namespace graph_analysis {
namespace algorithms {

MinCostFlow::MinCostFlow(const BaseGraph::Ptr& graph, Type type)
    : mpGraph(graph)
    , mType(type)
{}


uint32_t MinCostFlow::run()
{
    switch(mpGraph->getImplementationType())
    {
        typedef int32_t value_and_cost_t;
        typedef int32_t supply_t;

        case BaseGraph::LEMON_DIRECTED_GRAPH:
        {
            //graph_analysis::lemon::algorithms::MinCostFlow(mpGraph, type);

            graph_analysis::lemon::DirectedGraph::Ptr diGraph =
                dynamic_pointer_cast<graph_analysis::lemon::DirectedGraph>(mpGraph);
            assert(diGraph);

            typedef graph_analysis::lemon::DirectedGraph::graph_t::ArcMap<value_and_cost_t> ArcMap;
            typedef graph_analysis::lemon::DirectedGraph::graph_t::NodeMap<supply_t> NodeMap;

            ArcMap lowerMap(diGraph->raw());
            ArcMap upperMap(diGraph->raw());
            ArcMap costMap(diGraph->raw());

            EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
            while(edgeIt->next())
            {
                edge_t::Ptr edge = dynamic_pointer_cast< edge_t >(edgeIt->current());
                assert(edge);

                lowerMap[diGraph->getArc(edge)] = edge->getWeight(LOWER_BOUND);
                upperMap[diGraph->getArc(edge)] = edge->getWeight(UPPER_BOUND);
                costMap[diGraph->getArc(edge)]  = edge->getWeight(COST);
            }

            NodeMap supplyMap(diGraph->raw());

            VertexIterator::Ptr vertexIt = mpGraph->getVertexIterator();
            while(vertexIt->next())
            {
                vertex_t::Ptr vertex = dynamic_pointer_cast< vertex_t >(vertexIt->current());
                assert(vertex);
                supplyMap[diGraph->getNode(vertex)] = (supply_t) vertex->getWeight(SUPPLY_DEMAND);
            }

            typedef ::lemon::NetworkSimplex<graph_analysis::lemon::DirectedGraph::graph_t, value_and_cost_t> NetworkSimplex;
            NetworkSimplex simplex(diGraph->raw());

            // ArcMaps
            simplex.lowerMap(lowerMap);
            simplex.upperMap(upperMap);
            simplex.costMap(costMap);

            //NodeMap
            simplex.supplyMap(supplyMap);
            //simplex.stSupply( node s, node t, NetworkSimplex::Value k)
            switch( simplex.run() )
            {
                case NetworkSimplex::INFEASIBLE:
                    throw std::runtime_error("graph_analysis::algorithms::MinCostFlow: no feasible solution");
                case NetworkSimplex::OPTIMAL:
                {
                    EdgeIterator::Ptr edgeIt = mpGraph->getEdgeIterator();
                    while(edgeIt->next())
                    {
                        edge_t::Ptr edge = dynamic_pointer_cast< edge_t >(edgeIt->current());
                        assert(edge);

                        edge->setWeight( simplex.flow( diGraph->getArc(edge)), RESULT_FLOW );
                    }

                    VertexIterator::Ptr vertexIt = mpGraph->getVertexIterator();
                    while(vertexIt->next())
                    {
                        vertex_t::Ptr vertex = dynamic_pointer_cast< vertex_t >(vertexIt->current());
                        assert(vertex);
                        vertex->setWeight( simplex.potential( diGraph->getNode(vertex) ), RESULT_POTENTIAL);
                    }

                    return simplex.totalCost();
                }
                case NetworkSimplex::UNBOUNDED:
                    throw std::runtime_error("graph_analysis::algorithms::MinCostFlow: unbounded solution");
            }
            break;
        }
        case BaseGraph::BOOST_DIRECTED_GRAPH:
        case BaseGraph::SNAP_DIRECTED_GRAPH:
        default:
            throw std::runtime_error("graph_analysis::algorithms::MinCostFlow: not implemented for " + mpGraph->getImplementationTypeName());
            break;

    }
    return 0;
}

} // end namespace algorithms
} // end namespace graph_analysis
