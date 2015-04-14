#include <boost/test/unit_test.hpp>
#include <graph_analysis/gui/GVGraph.hpp>

using namespace graph_analysis;

/*
 * Note:
 * ----
 * 
 * These tests imply that clearNodes() shall not be called before clearEdges() -> otherwise exceptions are raised internally
 */

BOOST_AUTO_TEST_CASE(it_should_clear_edges_and_nodes)
{
    graph_analysis::gui::GVGraph *graph = new graph_analysis::gui::GVGraph("GVGraphWidget");
    graph->addNode(QString("1"));
    graph->addNode(QString("2"));
    graph->addNode(QString("3"));
    graph->addNode(QString("4"));

    graph->addEdge(QString("1"), QString("2"));
    graph->addEdge(QString("1"), QString("4"));
    graph->addEdge(QString("2"), QString("3"));
    graph->addEdge(QString("4"), QString("2"));
    graph->addEdge(QString("2"), QString("4"));

    std::cout << "it_should_clear_edges_and_nodes: graph->clearEdges();\n";
    graph->clearEdges();
    std::cout << "it_should_clear_edges_and_nodes: graph->clearNodes();\n";
    graph->clearNodes();
    delete graph;
}

BOOST_AUTO_TEST_CASE(it_should_clear_nodes_and_edges)
{
    graph_analysis::gui::GVGraph *graph = new graph_analysis::gui::GVGraph("GVGraphWidget");
    graph->addNode(QString("1"));
    graph->addNode(QString("2"));
    graph->addNode(QString("3"));
    graph->addNode(QString("4"));

    graph->addEdge(QString("1"), QString("2"));
    graph->addEdge(QString("1"), QString("4"));
    graph->addEdge(QString("2"), QString("3"));
    graph->addEdge(QString("4"), QString("2"));
    graph->addEdge(QString("2"), QString("4"));

    std::cout << "it_should_clear_nodes_and_edges: graph->clearNodes();\n";
    graph->clearNodes();
    std::cout << "it_should_clear_nodes_and_edges: graph->clearEdges();\n";
    graph->clearEdges();
    delete graph;
}

BOOST_AUTO_TEST_CASE(it_should_clear_nodes)
{
    graph_analysis::gui::GVGraph *graph = new graph_analysis::gui::GVGraph("GVGraphWidget");
    graph->addNode(QString("1"));
    graph->addNode(QString("2"));
    graph->addNode(QString("3"));
    graph->addNode(QString("4"));

    graph->addEdge(QString("1"), QString("2"));
    graph->addEdge(QString("1"), QString("4"));
    graph->addEdge(QString("2"), QString("3"));
    graph->addEdge(QString("4"), QString("2"));
    graph->addEdge(QString("2"), QString("4"));

    std::cout << "it_should_clear_nodes: graph->clearNodes();\n";
    graph->clearNodes();
    delete graph;
}

BOOST_AUTO_TEST_CASE(it_should_clear_edges)
{
    graph_analysis::gui::GVGraph *graph = new graph_analysis::gui::GVGraph("GVGraphWidget");
    graph->addNode(QString("1"));
    graph->addNode(QString("2"));
    graph->addNode(QString("3"));
    graph->addNode(QString("4"));

    graph->addEdge(QString("1"), QString("2"));
    graph->addEdge(QString("1"), QString("4"));
    graph->addEdge(QString("2"), QString("3"));
    graph->addEdge(QString("4"), QString("2"));
    graph->addEdge(QString("2"), QString("4"));

    std::cout << "it_should_clear_edges: graph->clearEdges();\n";
    graph->clearEdges();
    delete graph;
}
