#include <boost/test/unit_test.hpp>
#include <graph_analysis/WeightedEdge.hpp>
#include <graph_analysis/algorithms/DFS.hpp>

using namespace graph_analysis;
using namespace graph_analysis::algorithms;

BOOST_AUTO_TEST_SUITE(algorithms_dfs)

BOOST_AUTO_TEST_CASE(simple_dfs_1)
{
    graph_analysis::BaseGraph::Ptr graph = BaseGraph::getInstance();

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1a( new Vertex("1a"));
    Vertex::Ptr v1b( new Vertex("1b"));
    Vertex::Ptr v2a( new Vertex("2a"));
    Vertex::Ptr v2b( new Vertex("2b"));

    graph->addVertex(v0);
    graph->addVertex(v1a);
    graph->addVertex(v1b);
    graph->addVertex(v2a);
    graph->addVertex(v2b);

    WeightedEdge::Ptr e0(new WeightedEdge(30));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1a);

    WeightedEdge::Ptr e1(new WeightedEdge(30));
    e1->setSourceVertex(v0);
    e1->setTargetVertex(v1b);

    WeightedEdge::Ptr e2(new WeightedEdge(100.0));
    e2->setSourceVertex(v1a);
    e2->setTargetVertex(v2a);

    WeightedEdge::Ptr e3(new WeightedEdge(10.0));
    e3->setSourceVertex(v1b);
    e3->setTargetVertex(v2b);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);
    graph->addEdge(e3);


    DFSVisitor::Ptr visitor(new DFSVisitor());
    DFS dfs(graph, visitor);
    dfs.run(v0);

}

BOOST_AUTO_TEST_CASE(simple_dfs_2)
{
    graph_analysis::BaseGraph::Ptr graph = BaseGraph::getInstance();

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1a( new Vertex("1a"));
    Vertex::Ptr v1b( new Vertex("1b"));
    Vertex::Ptr v1c( new Vertex("1c"));
    Vertex::Ptr v1d( new Vertex("1d"));
    Vertex::Ptr v2a( new Vertex("2a"));
    Vertex::Ptr v2b( new Vertex("2b"));
    Vertex::Ptr v2c( new Vertex("2c"));
    Vertex::Ptr v3a( new Vertex("3a"));
    Vertex::Ptr v3b( new Vertex("3b"));
    Vertex::Ptr v4b( new Vertex("4b"));
    Vertex::Ptr vx( new Vertex("x"));

    graph->addVertex(v0);
    graph->addVertex(v1a);
    graph->addVertex(v1b);
    graph->addVertex(v1c);
    graph->addVertex(v1d);
    graph->addVertex(v2a);
    graph->addVertex(v2b);
    graph->addVertex(v2c);
    graph->addVertex(v3a);
    graph->addVertex(v3b);
    graph->addVertex(v4b);
    graph->addVertex(vx);

    WeightedEdge::Ptr e0(new WeightedEdge(30));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1a);

    WeightedEdge::Ptr e1(new WeightedEdge(40));
    e1->setSourceVertex(v0);
    e1->setTargetVertex(v1b);

    WeightedEdge::Ptr e2(new WeightedEdge(50));
    e2->setSourceVertex(v0);
    e2->setTargetVertex(v1c);

    WeightedEdge::Ptr e3(new WeightedEdge(60));
    e3->setSourceVertex(v0);
    e3->setTargetVertex(v1d);

    WeightedEdge::Ptr e4(new WeightedEdge(100.0));
    e4->setSourceVertex(v1a);
    e4->setTargetVertex(v2a);

    WeightedEdge::Ptr e5(new WeightedEdge(10.0));
    e5->setSourceVertex(v1b);
    e5->setTargetVertex(v2b);

    WeightedEdge::Ptr e6(new WeightedEdge(30));
    e6->setSourceVertex(v1c);
    e6->setTargetVertex(v2c);

    WeightedEdge::Ptr e7(new WeightedEdge(80));
    e7->setSourceVertex(v2a);
    e7->setTargetVertex(v3a);

    WeightedEdge::Ptr e8(new WeightedEdge(70));
    e8->setSourceVertex(v2b);
    e8->setTargetVertex(v3b);

    WeightedEdge::Ptr e9(new WeightedEdge(90));
    e9->setSourceVertex(v3b);
    e9->setTargetVertex(v4b);

    WeightedEdge::Ptr ex(new WeightedEdge(90));
    ex->setSourceVertex(v1a);
    ex->setTargetVertex(vx);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);
    graph->addEdge(e3);
    graph->addEdge(e4);
    graph->addEdge(e5);
    graph->addEdge(e6);
    graph->addEdge(e7);
    graph->addEdge(e8);
    graph->addEdge(e9);
    graph->addEdge(ex);


    DFSVisitor::Ptr visitor(new DFSVisitor());
    DFS dfs(graph, visitor);
    dfs.run(v0);

}
BOOST_AUTO_TEST_SUITE_END()
