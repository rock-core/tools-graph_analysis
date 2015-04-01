#include <boost/test/unit_test.hpp>
#include <graph_analysis/Percolation.hpp>
#include <graph_analysis/percolation/RandomNumberGenerator.hpp>
#include <graph_analysis/percolation/strategies/RandomDraw.hpp>
#include <graph_analysis/percolation/strategies/UniformRandomEdgeRemoval.hpp>
#include <graph_analysis/percolation/strategies/UniformRandomVertexRemoval.hpp>
#include <graph_analysis/lemon/Graph.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(percolation_test_suite)

BOOST_AUTO_TEST_CASE(random_number_generator)
{
    using namespace graph_analysis::percolation;

    {
        RandomNumberGenerator rng;
        for(int i = 0; i < 10; ++i)
        {
            BOOST_TEST_MESSAGE("Rand number for default seed:" << rng.getUniformPositiveNumber());
        }
    }
    sleep(1);
    {
        RandomNumberGenerator rng(0);
        for(int i = 0; i < 10; ++i)
        {
            BOOST_TEST_MESSAGE("Rand number for seed 0 :" << rng.getUniformPositiveNumber());
        }
    }
    {
        RandomNumberGenerator rng(10);
        for(int i = 0; i < 10; ++i)
        {
            BOOST_TEST_MESSAGE("Rand number for seed 10 :" << rng.getUniformPositiveNumber());
        }
    }

    {
        RandomNumberGenerator rng(10);
        for(int i = 0; i < 10; ++i)
        {
            BOOST_TEST_MESSAGE("Rand number for seed 10 :" << rng.getUniformPositiveNumber());
        }
    }
}

BOOST_AUTO_TEST_CASE(random_draw)
{
    using namespace graph_analysis::percolation::strategies;

    {
        RandomDraw randomDraw(1);
        BOOST_REQUIRE_THROW(randomDraw.drawItem(), std::runtime_error);

        randomDraw.setProbabilityDensity(0, 0.0);
        BOOST_REQUIRE_THROW(randomDraw.drawItem(), std::runtime_error);

        randomDraw.setProbabilityDensity(1, 2.0);
        GraphElementId id = randomDraw.drawItem();
        BOOST_REQUIRE_MESSAGE(id == 1, "Draw item with id 1, but got " << id);
        BOOST_REQUIRE_NO_THROW(randomDraw.removeItem(id));
        BOOST_REQUIRE_THROW(randomDraw.drawItem(), std::runtime_error);
    }

    {
        RandomDraw randomDraw;
        BOOST_REQUIRE_THROW(randomDraw.drawItem(), std::runtime_error);

        randomDraw.setProbabilityDensity(0, 0.0);
        randomDraw.setProbabilityDensity(1, 1.0);
        randomDraw.setProbabilityDensity(2, 10.0);
        std::map<int, int> drawCount;
        for(int i = 0; i < 100000; ++i)
        {
            GraphElementId id = randomDraw.drawItem();
            drawCount[id]++;
        }

        BOOST_REQUIRE_MESSAGE(drawCount[0] == 0, "Zero probability never drawn: " << drawCount[0]);
        BOOST_REQUIRE_MESSAGE(drawCount[1] < drawCount[2], "1 less drawn than 2");
        double factor = drawCount[2]/drawCount[1];
        BOOST_REQUIRE_MESSAGE( factor > 8, "2 more often draw than 1 : factor " << factor);
    }
}

BOOST_AUTO_TEST_CASE(random_uniform_edge_removal)
{
    BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());
    for(int i = 0; i < 1000; ++i)
    {
        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());

        Edge::Ptr e0(new Edge());
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        graph->addEdge(e0);
    }

    using namespace graph_analysis::percolation::strategies;

    {
        double occupationProbability = 1.0;
        percolation::Strategy::Ptr strategy(new UniformRandomEdgeRemoval(occupationProbability));

        percolation::Percolation percolation(graph);
        SubGraph::Ptr subgraph = percolation.apply(strategy);

        BOOST_REQUIRE_MESSAGE(subgraph->getVertexCount() == graph->getVertexCount(), "Subgraph vertex count: expected " << graph->getVertexCount() << " but got " << subgraph->getVertexCount()); 

        BOOST_REQUIRE_MESSAGE(subgraph->getEdgeCount() == graph->getEdgeCount(), "Subgraph edge count: expected " << graph->getEdgeCount() << " but got " << subgraph->getEdgeCount()); 
    }

    {
        double occupationProbability = 0.0;
        percolation::Strategy::Ptr strategy(new UniformRandomEdgeRemoval(occupationProbability));

        percolation::Percolation percolation(graph);
        SubGraph::Ptr subgraph = percolation.apply(strategy);

        BOOST_REQUIRE_MESSAGE(subgraph->getVertexCount() == graph->getVertexCount(), "Subgraph vertex count: expected " << graph->getVertexCount() << " but got " << subgraph->getVertexCount()); 

        BOOST_REQUIRE_MESSAGE(subgraph->getEdgeCount() == 0, "Subgraph edge count: expected 0 but got " << subgraph->getEdgeCount()); 
    }

    {
        double occupationProbability = 0.5;
        percolation::Strategy::Ptr strategy(new UniformRandomEdgeRemoval(occupationProbability));

        percolation::Percolation percolation(graph);
        SubGraph::Ptr subgraph = percolation.apply(strategy);

        BOOST_REQUIRE_MESSAGE(subgraph->getVertexCount() == graph->getVertexCount(), "Subgraph vertex count: expected " << graph->getVertexCount() << " but got " << subgraph->getVertexCount()); 

        BOOST_REQUIRE_MESSAGE(subgraph->getEdgeCount() < graph->getEdgeCount()*0.7, "Subgraph edge count: expected less than " << graph->getEdgeCount()*0.7 << " but got " << subgraph->getEdgeCount()); 
    }
}

BOOST_AUTO_TEST_CASE(random_uniform_vertex_removal)
{
    BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());
    for(int i = 0; i < 1000; ++i)
    {
        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());

        Edge::Ptr e0(new Edge());
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        graph->addEdge(e0);
    }

    using namespace graph_analysis::percolation::strategies;

    {
        double occupationProbability = 1.0;
        percolation::Strategy::Ptr strategy(new UniformRandomVertexRemoval(occupationProbability));

        percolation::Percolation percolation(graph);
        SubGraph::Ptr subgraph = percolation.apply(strategy);

        BOOST_REQUIRE_MESSAGE(subgraph->getVertexCount() == graph->getVertexCount(), "Subgraph vertex count: expected " << graph->getVertexCount() << " but got " << subgraph->getVertexCount()); 

        BOOST_REQUIRE_MESSAGE(subgraph->getEdgeCount() == graph->getEdgeCount(), "Subgraph edge count: expected " << graph->getEdgeCount() << " but got " << subgraph->getEdgeCount()); 
    }

    {
        double occupationProbability = 0.0;
        percolation::Strategy::Ptr strategy(new UniformRandomVertexRemoval(occupationProbability));

        percolation::Percolation percolation(graph);
        SubGraph::Ptr subgraph = percolation.apply(strategy);

        BOOST_REQUIRE_MESSAGE(subgraph->getVertexCount() == 0, "Subgraph vertex count: expected 0 but got " << subgraph->getVertexCount()); 

        BOOST_REQUIRE_MESSAGE(subgraph->getEdgeCount() == 0, "Subgraph edge count: expected 0 but got " << subgraph->getEdgeCount()); 
    }

    {
        double occupationProbability = 0.5;
        percolation::Strategy::Ptr strategy(new UniformRandomVertexRemoval(occupationProbability));

        percolation::Percolation percolation(graph);
        SubGraph::Ptr subgraph = percolation.apply(strategy);

        BOOST_REQUIRE_MESSAGE(subgraph->getVertexCount() < graph->getVertexCount()*0.7, "Subgraph vertex count: expected less than " << graph->getVertexCount()*0.7 << " but got " << subgraph->getVertexCount()); 

        BOOST_REQUIRE_MESSAGE(subgraph->getEdgeCount() < graph->getEdgeCount()*0.7, "Subgraph edge count: expected less than " << graph->getEdgeCount()*0.7 << " but got " << subgraph->getEdgeCount()); 
    }
}

BOOST_AUTO_TEST_SUITE_END()
