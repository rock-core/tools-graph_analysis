#include <boost/test/unit_test.hpp>
#include <graph_analysis/BaseGraph.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(subgraph_test_suite)

BOOST_AUTO_TEST_CASE(create_sub_graph)
{
    BaseGraph::Ptr graph = BaseGraph::getInstance(BaseGraph::LEMON_DIRECTED_GRAPH);

    Vertex::Ptr v0( new Vertex());
    Vertex::Ptr v1( new Vertex());

    Edge::Ptr e0(new Edge());
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    graph->addEdge(e0);

    {
        SubGraph::Ptr subgraph = BaseGraph::getSubGraph(graph);
        {
            VertexIterator::Ptr vertexIterator = subgraph->getVertexIterator();
            int count = 0;
            while(vertexIterator->next())
            {
                count++;
            }
            BOOST_REQUIRE_MESSAGE(count == 2, "Expected 2 vertices, found " << count);
        }

        {
            EdgeIterator::Ptr edgeIterator = subgraph->getEdgeIterator();
            int count = 0;
            while(edgeIterator->next())
            {
                count++;
            }
            BOOST_REQUIRE_MESSAGE(count == 1, "Expected 1 edge, found " << count);
        }
    }

    {
        SubGraph::Ptr subgraph = BaseGraph::getSubGraph(graph);
        subgraph->disable(e0);
        {
            VertexIterator::Ptr vertexIterator = subgraph->getVertexIterator();
            int count = 0;
            while(vertexIterator->next())
            {
                count++;
            }
            BOOST_REQUIRE_MESSAGE(count == 2, "Expected 2 vertices, found " << count);
        }

        {
            EdgeIterator::Ptr edgeIterator = subgraph->getEdgeIterator();
            int count = 0;
            while(edgeIterator->next())
            {
                count++;
            }
            BOOST_REQUIRE_MESSAGE(count == 0, "Expected 0 edge, found " << count);
        }
    }
    {
        SubGraph::Ptr subgraph = BaseGraph::getSubGraph(graph);
        subgraph->disable(v0);
        {
            VertexIterator::Ptr vertexIterator = subgraph->getVertexIterator();
            int count = 0;
            while(vertexIterator->next())
            {
                count++;
            }
            BOOST_REQUIRE_MESSAGE(count == 1, "Expected 1 vertex, found " << count);
        }
        {
            EdgeIterator::Ptr edgeIterator = subgraph->getEdgeIterator();
            int count = 0;
            while(edgeIterator->next())
            {
                count++;
            }
            BOOST_REQUIRE_MESSAGE(count == 0, "Expected 0 edge, found " << count);
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()
