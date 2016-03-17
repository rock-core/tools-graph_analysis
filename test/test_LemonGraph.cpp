#include <boost/test/unit_test.hpp>
#include <graph_analysis/Edge.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/filters/EdgeContextFilter.hpp>


using namespace graph_analysis;
namespace gl = graph_analysis::lemon;

BOOST_AUTO_TEST_SUITE(lemon_graph)

BOOST_AUTO_TEST_CASE(layouting)
{
//    BOOST_REQUIRE_MESSAGE(true, "Now Testing layouting issues with Lemon Graphs: " << "started");
    std::set<std::string> types = VertexTypeManager::getInstance()->getSupportedTypes();
    BOOST_REQUIRE_MESSAGE(false, "ACTUALLY THIS TEST CASE IS BROKEN -- FIX THE TEST");
    BOOST_REQUIRE_MESSAGE(types.end() != types.find("cluster"), "lemon_graph Testing | layouting: testing type 'cluster' is registered");

    graph_analysis::BaseGraph::Ptr mpLayoutingGraph = BaseGraph::Ptr( new gl::DirectedGraph() );
    graph_analysis::BaseGraph::Ptr mpGraph          = BaseGraph::Ptr( new gl::DirectedGraph() );
    io::GVGraph* mpGVGraphPost = new io::GVGraph(mpLayoutingGraph, "GVGraphWidget");

    Vertex::Ptr v0 = VertexTypeManager::getInstance()->createVertex("cluster", "v0"); // (new Vertex("v0"));
    Vertex::Ptr v1 = VertexTypeManager::getInstance()->createVertex("cluster", "v1"); // (new Vertex("v1"));
    Vertex::Ptr v2 = VertexTypeManager::getInstance()->createVertex("cluster", "v2"); // (new Vertex("v2"));
    Vertex::Ptr v3 = VertexTypeManager::getInstance()->createVertex("cluster", "v3"); // (new Vertex("v3"));
    Vertex::Ptr v4 = VertexTypeManager::getInstance()->createVertex("cluster", "v4"); // (new Vertex("v4"));

    BOOST_REQUIRE_MESSAGE(v0.get() && v1.get() && v2.get() && v3.get() && v4.get(), "lemon_graph Testing | layouting: testing vertices pointers did come back null");
    BOOST_REQUIRE_MESSAGE("v0" == v0->getLabel(), "Checking vertex label to be 'v0'; instead was found: " << v0->getLabel());
    BOOST_REQUIRE_MESSAGE("v1" == v1->getLabel(), "Checking vertex label to be 'v1'; instead was found: " << v1->getLabel());
    BOOST_REQUIRE_MESSAGE("v2" == v2->getLabel(), "Checking vertex label to be 'v2'; instead was found: " << v2->getLabel());
    BOOST_REQUIRE_MESSAGE("v3" == v3->getLabel(), "Checking vertex label to be 'v3'; instead was found: " << v3->getLabel());
    BOOST_REQUIRE_MESSAGE("v4" == v4->getLabel(), "Checking vertex label to be 'v4'; instead was found: " << v4->getLabel());

    mpGraph->addVertex(v0);
    mpGraph->addVertex(v1);
    mpGraph->addVertex(v2);
    mpGraph->addVertex(v3);
    mpGraph->addVertex(v4);
    mpLayoutingGraph->addVertex(v0);
    mpLayoutingGraph->addVertex(v1);
    mpLayoutingGraph->addVertex(v2);
    mpLayoutingGraph->addVertex(v3);
    mpLayoutingGraph->addVertex(v4);

    io::GVGraph* mpGVGraphPre  = new io::GVGraph(mpGraph, "GVGraphWidget");

    mpGVGraphPost->addNode(v0);
    mpGVGraphPost->addNode(v1);
    mpGVGraphPost->addNode(v2);
    mpGVGraphPost->addNode(v3);
    mpGVGraphPost->addNode(v4);

    mpGVGraphPre->addNode(v0);
    mpGVGraphPre->addNode(v1);
    mpGVGraphPre->addNode(v2);
    mpGVGraphPre->addNode(v3);
    mpGVGraphPre->addNode(v4);
}

BOOST_AUTO_TEST_SUITE_END()
