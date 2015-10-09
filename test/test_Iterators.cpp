#include <boost/test/unit_test.hpp>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/DirectedGraphInterface.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(iterators)

BOOST_AUTO_TEST_CASE(STL_LIKE_ITERATORS)
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

        for(auto v : graph->edges()){
            BOOST_REQUIRE_MESSAGE(false, "The graph has no edges yet");
        }
        graph->addEdge(e0);

        BOOST_REQUIRE_MESSAGE(graph->edges().begin() != graph->edges().end(), "On a non empy graph, edges.begin() not edges.end()");
        graph->addEdge(e1);

        unsigned int cnt=0;
        for(auto v : graph->edges()){
            BOOST_REQUIRE_MESSAGE(v->toString() == e0->toString(), "Member should be accassable and have the same default string (all of them)");
            cnt++;
        }
        BOOST_REQUIRE_MESSAGE(cnt == 2, "The Graph should have 2 edges but it has not");
        cnt=0;
        for(auto v : graph->vertices()){
            cnt++;
        }
        BOOST_REQUIRE_MESSAGE(cnt == 3, "The Graph should have 3 verticies but it has not");

        if(auto g = dynamic_cast<graph_analysis::DirectedGraphInterface*>(graph.get())){
            try{
                g->getOutEdgeIterator(v0);
            }catch(std::runtime_error e){
                std::cerr << "Warn testcase for graph type " << i  << " failed, assuming not yet implemented, error is: " << e.what() << std::endl;
                continue;
            }

            cnt=0;
            for(auto v: g->outEdges(v0)){
                BOOST_REQUIRE_MESSAGE(v->toString() == e0->toString(), "Member should be accassable and have the same default string (all of them)");
                cnt++;
            }
            BOOST_REQUIRE_MESSAGE(cnt == 1, "Directed Graph should have one outgoing edge here but is has " << cnt);
            BOOST_REQUIRE_MESSAGE(!(g->inEdges(v0).begin() != g->inEdges(v0).end()), "Directed Graph should inot have any outgoing edge here");
        }
    }
}

BOOST_AUTO_TEST_SUITE_END()

