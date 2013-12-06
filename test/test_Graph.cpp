#include <boost/test/unit_test.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/CommonFilters.hpp>

using namespace graph_analysis::lemon;

BOOST_AUTO_TEST_CASE(it_should_instanciate)
{
    graph_analysis::lemon::DirectedGraph graph;
}


BOOST_AUTO_TEST_CASE(it_should_allow_applying_a_view)
{
    DirectedGraph graph;

    typedef DirectedGraph Graph;
    typedef DirectedGraph::Vertex Vertex;
    typedef DirectedGraph::Edge Edge;

    typedef typename DirectedGraph::VertexPropertyType VertexData;
    typedef DirectedGraph::EdgePropertyType EdgeData;

    Vertex v0 = graph.addVertex();
    Vertex v1 = graph.addVertex();

    VertexData::Ptr vertexData0(new DirectedGraph::VertexProperty());
    VertexData::Ptr vertexData1(new DirectedGraph::VertexProperty());

    graph.assignVertexProperty(v0, vertexData0);
    BOOST_REQUIRE_MESSAGE( v0 == vertexData0->getVertex(&graph), "Retrieve assigned vertex");

    graph.assignVertexProperty(v1, vertexData1);
    BOOST_REQUIRE_MESSAGE( v1 == vertexData1->getVertex(&graph), "Retrieve assigned vertex");


    EdgeData::Ptr edgeData0(new DirectedGraph::EdgeProperty());
    Edge e = graph.addEdge(v0,v1);

    graph.assignEdgeProperty(e, edgeData0);
    BOOST_REQUIRE_MESSAGE(e == edgeData0->getEdge(&graph), "Assigned edge can be retrieved");


    // Set graph view
    {
        graph_analysis::GraphView<Graph,VertexData::Ptr, EdgeData::Ptr> gv;
        graph_analysis::Filter< VertexData::Ptr >::Ptr filter(new graph_analysis::filter::PermitAll< VertexData::Ptr >() );
        gv.setVertexFilter(filter);

        DirectedSubGraph subGraph = gv.apply(graph);
        int subgraphCount = ::lemon::countNodes(subGraph.raw());
        int graphCount = ::lemon::countNodes(graph.raw());
        BOOST_REQUIRE_MESSAGE( subgraphCount == graphCount, "Subgraph contains all nodes after applying PermitAll filter: " << subgraphCount << " vs. " << graphCount);
    }

    {
        graph_analysis::GraphView<Graph,VertexData::Ptr, EdgeData::Ptr> gv;
        graph_analysis::Filter< VertexData::Ptr >::Ptr filter(new graph_analysis::filter::DenyAll< VertexData::Ptr >() );
        gv.setVertexFilter(filter);

        DirectedSubGraph subGraph = gv.apply(graph);

        int subgraphCount = ::lemon::countNodes(subGraph.raw());
        BOOST_REQUIRE_MESSAGE( subgraphCount == 0, "Subgraph contains no nodes after applying DenyAll filter" << subgraphCount << "vs. 0 " );
    }
}
