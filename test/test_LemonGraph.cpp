#include <boost/test/unit_test.hpp>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>


using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(lemon_graph)

BOOST_AUTO_TEST_CASE(layouting)
{
//    BOOST_REQUIRE_MESSAGE(true, "Now Testing layouting issues with Lemon Graphs: " << "started");
    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    BOOST_REQUIRE_MESSAGE(types.end() != types.find("cluster"), "lemon_graph Testing | layouting: testing type 'cluster' is registered");

    graph_analysis::BaseGraph::Ptr mpLayoutingGraph;
    io::GVGraph* mpGVGraph = new io::GVGraph(mpLayoutingGraph, "GVGraphWidget");


    Vertex::Ptr v0 = VertexTypeManager::getInstance()->createVertex("cluster", "v0"); // (new Vertex("v0"));
    Vertex::Ptr v1 = VertexTypeManager::getInstance()->createVertex("cluster", "v1"); // (new Vertex("v1"));
    Vertex::Ptr v2 = VertexTypeManager::getInstance()->createVertex("cluster", "v2"); // (new Vertex("v2"));
    Vertex::Ptr v3 = VertexTypeManager::getInstance()->createVertex("cluster", "v3"); // (new Vertex("v3"));
    Vertex::Ptr v4 = VertexTypeManager::getInstance()->createVertex("cluster", "v4"); // (new Vertex("v4"));

    BOOST_REQUIRE_MESSAGE(v0.get() && v1.get() && v2.get() && v3.get() && v4.get(), "lemon_graph Testing | layouting: testing vertices pointers did come back null");

    mpLayoutingGraph->addVertex(v0);
    /*
    mpLayoutingGraph->addVertex(v1);
    mpLayoutingGraph->addVertex(v2);
    mpLayoutingGraph->addVertex(v3);
    mpLayoutingGraph->addVertex(v4);
    mpGVGraph->addNode(v0);
    mpGVGraph->addNode(v1);
    mpGVGraph->addNode(v2);
    mpGVGraph->addNode(v3);
    mpGVGraph->addNode(v4);
     */
}

BOOST_AUTO_TEST_SUITE_END()
