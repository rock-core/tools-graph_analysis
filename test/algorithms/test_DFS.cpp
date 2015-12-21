#include <boost/test/unit_test.hpp>
#include <graph_analysis/WeightedEdge.hpp>
#include <graph_analysis/algorithms/DFS.hpp>

using namespace graph_analysis;
using namespace graph_analysis::algorithms;

BOOST_AUTO_TEST_SUITE(algorithms_dfs)

BOOST_AUTO_TEST_CASE(simple)
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
BOOST_AUTO_TEST_SUITE_END()
