#include <boost/test/unit_test.hpp>
#include <graph_analysis/LemonGraph.hpp>
#include <graph_analysis/GraphView.hpp>

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
    graph.assignVertexProperty(v1, vertexData1);

    EdgeData::Ptr edgeData0(new DirectedGraph::EdgeProperty());

    Edge e = graph.addEdge(v0,v1);
    graph.assignEdgeProperty(e, edgeData0);


    // Set graph view
    graph_analysis::GraphView<Graph,VertexData::Ptr, EdgeData::Ptr> gv;

    graph_analysis::Filter< VertexData::Ptr >::Ptr filter(new graph_analysis::Filter< VertexData::Ptr >() );
    gv.setVertexFilter(filter);

    DirectedGraph::SubGraph subGraph = gv.apply(graph);

    ::lemon::countNodes(subGraph);
}
