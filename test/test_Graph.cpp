#include <boost/test/unit_test.hpp>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/GraphView.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_CASE(it_should_instanciate)
{
    graph_analysis::DirectedGraph graph;
}


BOOST_AUTO_TEST_CASE(it_should_allow_applying_a_view)
{
    DirectedGraph graph;

    typedef graph_analysis::DirectedGraph Graph;
    typedef graph_analysis::DirectedGraph::Vertex Vertex;
    typedef graph_analysis::DirectedGraph::Edge Edge;

    typedef typename graph_analysis::DirectedGraph::VertexPropertyType VertexData;
    typedef graph_analysis::DirectedGraph::EdgePropertyType EdgeData;

    Vertex v0 = graph.addVertex();
    Vertex v1 = graph.addVertex();

    VertexData::Ptr vertexData0(new VertexProperty<Vertex>());
    VertexData::Ptr vertexData1(new VertexProperty<Vertex>());

    graph.assignVertexProperty(v0, vertexData0);
    graph.assignVertexProperty(v1, vertexData1);

    EdgeData::Ptr edgeData0(new EdgeProperty<Edge>());

    Edge e = graph.addEdge(v0,v1);
    graph.assignEdgeProperty(e, edgeData0);


    // Set graph view
    GraphView<Graph,VertexData::Ptr, EdgeData::Ptr> gv;

    Filter< VertexData::Ptr >::Ptr filter(new Filter< VertexData::Ptr >() );
    gv.setVertexFilter(filter);

    DirectedGraph::SubGraph subGraph = gv.apply(graph);

    lemon::countNodes(subGraph);
}
