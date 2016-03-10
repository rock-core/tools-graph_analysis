#include <boost/test/unit_test.hpp>
#include <graph_analysis/WeightedEdge.hpp>
#include <graph_analysis/algorithms/Delta.hpp>
#include <graph_analysis/GraphIO.hpp>

using namespace graph_analysis;
using namespace graph_analysis::algorithms;

BOOST_AUTO_TEST_SUITE(algorithms_delta)

BOOST_AUTO_TEST_CASE(simple_delta)
{
    graph_analysis::BaseGraph::Ptr graph1 = BaseGraph::getInstance();
    graph_analysis::BaseGraph::Ptr graph2 = BaseGraph::getInstance();

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1( new Vertex("1"));
    Vertex::Ptr v2( new Vertex("2"));
    Vertex::Ptr v3( new Vertex("3"));

    graph1->addVertex(v0);
    graph1->addVertex(v1);
    graph1->addVertex(v2);
    graph1->addVertex(v3);

    WeightedEdge::Ptr e0(new WeightedEdge(10));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    WeightedEdge::Ptr e1(new WeightedEdge(20));
    e1->setSourceVertex(v1);
    e1->setTargetVertex(v3);

    graph1->addEdge(e0);
    graph1->addEdge(e1);
    
    graph_analysis::io::GraphIO::write("/tmp/test-delta_sourceGraph.dot", graph1);

    Vertex::Ptr v4( new Vertex("4"));
    graph2->addVertex(v1);
    graph2->addVertex(v2);
    graph2->addVertex(v3);
    graph2->addVertex(v4);

    WeightedEdge::Ptr e2(new WeightedEdge(30));
    e2->setSourceVertex(v1);
    e2->setTargetVertex(v2);

    WeightedEdge::Ptr e3(new WeightedEdge(40));
    e3->setSourceVertex(v1);
    e3->setTargetVertex(v4);

    WeightedEdge::Ptr e4(new WeightedEdge(50));
    e4->setSourceVertex(v2);
    e4->setTargetVertex(v3);

    graph2->addEdge(e2);
    graph2->addEdge(e3);
    graph2->addEdge(e4);

    graph_analysis::io::GraphIO::write("/tmp/test-delta_targetGraph.dot", graph2);
        

    Delta delta(graph1,graph2);
    delta.run();
    int i = 0;

    BOOST_REQUIRE_MESSAGE(delta.hasNext(), "Expected True, Actual: " << delta.hasNext());

    while (delta.hasNext())
    {
        i++;
        std::stringstream ss;
        ss << "/tmp/test-delta_getNext_" << i << ".dot";
        graph_analysis::io::GraphIO::write(ss.str(), delta.getNext());
    }

    BOOST_REQUIRE_THROW(delta.getNext(), std::runtime_error);
}

BOOST_AUTO_TEST_SUITE_END()