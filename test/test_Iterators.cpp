#include <boost/test/unit_test.hpp>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/DirectedGraphInterface.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(iterators)

BOOST_AUTO_TEST_CASE(lemon_like)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));

        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());
        Vertex::Ptr v2( new Vertex());

        Edge::Ptr e0(new Edge());
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        Edge::Ptr e1(new Edge());
        e1->setSourceVertex(v1);
        e1->setTargetVertex(v2);

        {
            EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
            while(edgeIt->next())
            {
                BOOST_REQUIRE_MESSAGE(false, "The graph has no edges yet");
            }
        }
        graph->addEdge(e0);
        graph->addEdge(e1);

        {
            unsigned int cnt=0;
            EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
            while(edgeIt->next())
            {
                Edge::Ptr edge = edgeIt->current();
                BOOST_REQUIRE_MESSAGE(edge->toString() == e0->toString(), "Member should be accessible and have the same default string (all of them)");
                cnt++;
            }
            BOOST_REQUIRE_MESSAGE(cnt == 2, "The Graph should have 2 edges but it has " << cnt);
        }

        {
            unsigned int cnt=0;
            VertexIterator::Ptr vertexIt = graph->getVertexIterator();
            while(vertexIt->next())
            {
                Vertex::Ptr vertex = vertexIt->current();
                BOOST_REQUIRE_MESSAGE(vertex->toString() == v1->toString(), "Member should be accessible and have the same default string (all of them)");
                ++cnt;
            }
            BOOST_REQUIRE_MESSAGE(cnt == 3, "The Graph should have 3 vertices but it has " << cnt);

            graph_analysis::DirectedGraphInterface::Ptr directedGraph = dynamic_pointer_cast<graph_analysis::DirectedGraphInterface>(graph);
            if(directedGraph)
            {
                try{
                    directedGraph->getOutEdgeIterator(v0);
                } catch(std::runtime_error e)
                {
                    BOOST_CHECK_MESSAGE(false, "Testcase for graph type " << i  << " failed, assuming not yet implemented, error is: " << e.what());
                    continue;
                }

                cnt=0;
                unsigned int cnt=0;
                EdgeIterator::Ptr edgeIt = directedGraph->getOutEdgeIterator(v0);
                while(edgeIt->next())
                {
                    Edge::Ptr edge = edgeIt->current();
                    BOOST_REQUIRE_MESSAGE(edge->toString() == e0->toString(), "Member should be accessible and have the same default string (all of them)");
                    ++cnt;
                }

                BOOST_REQUIRE_MESSAGE(cnt == 1, "Directed Graph should have one outgoing edge here but is has " << cnt);
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(stl_like)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));

        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());
        Vertex::Ptr v2( new Vertex());

        Edge::Ptr e0(new Edge());
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        Edge::Ptr e1(new Edge());
        e1->setSourceVertex(v1);
        e1->setTargetVertex(v2);

        BOOST_REQUIRE_MESSAGE(!(graph->edges().begin() != graph->edges().end()), "On a Empy graph, edges.begin() not equal edges.end()");

        {
            typedef BaseIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr> IterableEdges;
            IterableEdges::Iterator eit = graph->edges().begin();
            for(; eit != graph->edges().end(); ++eit)
            {
                BOOST_REQUIRE_MESSAGE(false, "The graph has no edges yet");
            }
        }
        graph->addEdge(e0);

        BOOST_REQUIRE_MESSAGE(graph->edges().begin() != graph->edges().end(), "On a non empty graph, edges.begin() not edges.end()");
        graph->addEdge(e1);

        {
            unsigned int cnt=0;
            typedef BaseIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr> IterableEdges;
            IterableEdges::Iterator eit = graph->edges().begin();
            for(; eit != graph->edges().end(); ++eit)
            {
                BOOST_REQUIRE_MESSAGE((*eit)->toString() == e0->toString(), "Member should be accessible and have the same default string (all of them)");
                cnt++;
            }
            BOOST_REQUIRE_MESSAGE(cnt == 2, "The Graph should have 2 edges but it has not");
        }

        {
            unsigned int cnt=0;
            typedef BaseIterable<VertexIterator::Ptr, BaseGraph, Vertex::Ptr> IterableVertices;
            IterableVertices::Iterator vit = graph->vertices().begin();
            for(; vit != graph->vertices().end(); ++vit)
            {
                ++cnt;
            }
            BOOST_REQUIRE_MESSAGE(cnt == 3, "The Graph should have 3 vertices but it has not");

            graph_analysis::DirectedGraphInterface::Ptr directedGraph = dynamic_pointer_cast<graph_analysis::DirectedGraphInterface>(graph);
            if(directedGraph)
            {
                try{
                    directedGraph->getOutEdgeIterator(v0);
                } catch(std::runtime_error e)
                {
                    BOOST_CHECK_MESSAGE(false, "Testcase for graph type " << i  << " failed, assuming not yet implemented, error is: " << e.what());
                    continue;
                }

                cnt=0;
                typedef SpecializedIterable<EdgeIterator::Ptr, DirectedGraphInterface, Edge::Ptr, Vertex::Ptr> IterableOutEdges;
                IterableOutEdges::Iterator eit = directedGraph->outEdges(v0).begin();
                for(; eit != directedGraph->outEdges(v0).end(); ++eit)
                {
                    Edge::Ptr edge = *eit;
                    BOOST_REQUIRE_MESSAGE(edge->toString() == e0->toString(), "Member should be accessible and have the same default string (all of them)");
                    ++cnt;
                }

                BOOST_REQUIRE_MESSAGE(cnt == 1, "Directed Graph should have one outgoing edge here but is has " << cnt);
                BOOST_REQUIRE_MESSAGE(!(directedGraph->inEdges(v0).begin() != directedGraph->inEdges(v0).end()), "Directed Graph should not have any incoming edge here");
            }
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

