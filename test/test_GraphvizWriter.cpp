#include <boost/test/unit_test.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>

using namespace graph_analysis;
#define FILENAME "testGraph.dot"

/*
 * Note:
 * ----
 * These tests imply that GraphvizWriter class works correctly - it constructs a complete graph with 4 nodes 
 * and renders it to file of hardwired name "testGraph.dot"
 * (manual dot rendeing of the output .dot file and checking it is assumed)
 */

BOOST_AUTO_TEST_CASE(it_should_output_dot_file_of_4Node_CompleteGraph)
{
    BOOST_REQUIRE_MESSAGE(true, "constructing test graph");
    graph_analysis::BaseGraph *graph = new graph_analysis::lemon::DirectedGraph();
    BOOST_REQUIRE_MESSAGE(true, "    constructing 4 vertices");
    Vertex::Ptr v0(new Vertex("0"));
    Vertex::Ptr v1(new Vertex("1"));
    Vertex::Ptr v2(new Vertex("2"));
    Vertex::Ptr v3(new Vertex("3"));

    BOOST_REQUIRE_MESSAGE(true, "    constructing 6 edges");
    Edge::Ptr e0(new Edge(v0, v1));
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

    BOOST_REQUIRE_MESSAGE(true, "Starting rendering to file " << FILENAME);
    io::GraphvizWriter* writer = new io::GraphvizWriter();
    writer->write(FILENAME, graph);
    BOOST_REQUIRE_MESSAGE(true, "Rendering completed. Please manually check a complete graph with 4-nodes is contained in file " << FILENAME);

    delete writer;
}
