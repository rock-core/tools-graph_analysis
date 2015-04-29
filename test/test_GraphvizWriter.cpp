#include <boost/test/unit_test.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(graphviz_writer)
/*
 * Note:
 * ----
 * These tests imply that GraphvizWriter class works correctly - it constructs a complete graph with 4 nodes 
 * and renders it to file of hardwired name "testGraph.dot"
 * (manual dot rendeing of the output .dot file and checking it is assumed)
 */

BOOST_AUTO_TEST_CASE(dot)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        BOOST_REQUIRE_MESSAGE(true, "constructing test graph");
        BOOST_REQUIRE_MESSAGE(true, "    constructing 4 vertices");
        Vertex::Ptr v0(new Vertex("label 0"));
        Vertex::Ptr v1(new Vertex("label 1"));
        Vertex::Ptr v2(new Vertex("label 2"));
        Vertex::Ptr v3(new Vertex("label 3"));

        BOOST_REQUIRE_MESSAGE(true, "    constructing 6 edges");
        Edge::Ptr e0(new Edge(v0, v1, "edge label 0"));
        Edge::Ptr e1(new Edge(v0, v2));
        Edge::Ptr e2(new Edge(v0, v3));
        Edge::Ptr e3(new Edge(v1, v2));
        Edge::Ptr e4(new Edge(v1, v3));
        Edge::Ptr e5(new Edge(v2, v3));

        BOOST_REQUIRE_MESSAGE(true, "    adding the 4 vertices to graph");
        graph->addVertex(v0);
        graph->addVertex(v1);
        graph->addVertex(v2);
        graph->addVertex(v3);

        BOOST_REQUIRE_MESSAGE(true, "    adding the 6 edges to graph");
        graph->addEdge(e0);
        graph->addEdge(e1);
        graph->addEdge(e2);
        graph->addEdge(e3);
        graph->addEdge(e4);
        graph->addEdge(e5);
        BOOST_REQUIRE_MESSAGE(true, "constructed test graph successfully");

        std::string filename = "/tmp/test-" + graph->getImplementationTypeName() + "-graphviz.dot";
        BOOST_REQUIRE_MESSAGE(true, "Starting rendering to file " << filename);
        io::GraphIO::write(filename, *graph, representation::GRAPHVIZ);
        BOOST_REQUIRE_MESSAGE(true, "Rendering completed. Please manually check a complete graph with 4-nodes is contained in file " << filename);
    }
}

BOOST_AUTO_TEST_SUITE_END()
