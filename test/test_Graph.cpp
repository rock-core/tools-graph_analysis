#include <boost/test/unit_test.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/snap/Graph.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/filters/CommonFilters.hpp>

#include <graph_analysis/GraphIO.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_CASE(it_should_instanciate)
{
    graph_analysis::lemon::DirectedGraph graph;
}


BOOST_AUTO_TEST_CASE(it_should_work_for_lemon)
{
    ::graph_analysis::lemon::DirectedGraph graph;

    Vertex::Ptr v0( new Vertex());
    Vertex::Ptr v1( new Vertex());
    Vertex::Ptr v2( new Vertex());
    Vertex::Ptr v3( new Vertex());

    BOOST_REQUIRE_THROW( v0->getId(graph.getId()), std::runtime_error);
    BOOST_REQUIRE_THROW( v1->getId(graph.getId()), std::runtime_error);
    BOOST_REQUIRE_THROW( v2->getId(graph.getId()), std::runtime_error);
    BOOST_REQUIRE_THROW( v3->getId(graph.getId()), std::runtime_error);

    graph.addVertex(v0);
    graph.addVertex(v1);
    graph.addVertex(v2);
    graph.addVertex(v3);

    BOOST_REQUIRE_NO_THROW(v0->getId(graph.getId()));
    BOOST_REQUIRE_NO_THROW(v1->getId(graph.getId()));
    BOOST_REQUIRE_NO_THROW(v2->getId(graph.getId()));
    BOOST_REQUIRE_NO_THROW(v3->getId(graph.getId()));

    Edge::Ptr e0(new Edge());
    BOOST_REQUIRE_THROW( e0->getId(graph.getId()), std::runtime_error);
    BOOST_REQUIRE_THROW(graph.addEdge(e0), std::runtime_error);
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);
    BOOST_REQUIRE_NO_THROW(graph.addEdge(e0));
    BOOST_REQUIRE_MESSAGE( e0->getSourceVertex()->getUid() != e0->getTargetVertex()->getUid(), "Edge should not point to same node");

    Edge::Ptr e1(new Edge());
    BOOST_REQUIRE_THROW( e1->getId(graph.getId()), std::runtime_error);
    BOOST_REQUIRE_THROW(graph.addEdge(e1), std::runtime_error);
    e1->setSourceVertex(v2);
    e1->setTargetVertex(v3);
    BOOST_REQUIRE_NO_THROW(graph.addEdge(e1));
    BOOST_REQUIRE_MESSAGE( e1->getSourceVertex()->getUid() != e1->getTargetVertex()->getUid(), "Edge should not point to same node");

    graph.write();

    int graphCount = ::lemon::countNodes(graph.raw());
    BOOST_REQUIRE_MESSAGE( graphCount == 4, "Graph contains all nodes");

    ::graph_analysis::lemon::DirectedGraph copiedGraph = graph;
    int copiedGraphCount = ::lemon::countNodes(copiedGraph.raw());
    BOOST_REQUIRE_MESSAGE( graphCount == copiedGraphCount, "Graph copy contains all nodes " << copiedGraphCount << " of " << graphCount);

    VertexIterator::Ptr nodeIt = graph.getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex0 = nodeIt->current();
        BOOST_REQUIRE_MESSAGE( vertex0->toString() != "", "Vertex: " << vertex0->toString() );
    }

    EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge0 = edgeIt->current();
        BOOST_REQUIRE_MESSAGE( edge0->toString() != "", "Edge: " << edge0->toString() );
    }

    // Set graph view
    {
        graph_analysis::GraphView< ::graph_analysis::lemon::DirectedGraph > gv;
        graph_analysis::Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filter::PermitAll< Vertex::Ptr >() );
        gv.setVertexFilter(filter);

        ::graph_analysis::lemon::DirectedSubGraph subGraph = gv.apply(graph);
        int subgraphCount = ::lemon::countNodes(subGraph.raw());
        int graphCount = ::lemon::countNodes(graph.raw());
        BOOST_REQUIRE_MESSAGE( subgraphCount == graphCount, "Subgraph contains all nodes after applying PermitAll filter: '" << subgraphCount << "' vs. '" << graphCount << "'");
    }

    {
        graph_analysis::GraphView< ::graph_analysis::lemon::DirectedGraph> gv;
        graph_analysis::Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filter::DenyAll< Vertex::Ptr >() );
        gv.setVertexFilter(filter);

        ::graph_analysis::lemon::DirectedSubGraph subGraph = gv.apply(graph);

        int subgraphCount = ::lemon::countNodes(subGraph.raw());
        BOOST_REQUIRE_MESSAGE( subgraphCount == 0, "Subgraph contains no nodes after applying DenyAll filter '" << subgraphCount << "' vs. '0'" );
    }

    {
        graph_analysis::GraphView< ::graph_analysis::lemon::DirectedGraph> gv;
        graph_analysis::Filter< Vertex::Ptr >::Ptr vertexFilter(new graph_analysis::filter::PermitAll< Vertex::Ptr >() );
        gv.setVertexFilter(vertexFilter);

        graph_analysis::Filter< Edge::Ptr >::Ptr edgeFilter(new graph_analysis::filter::PermitAll< Edge::Ptr >() );
        gv.setEdgeFilter(edgeFilter);


        ::graph_analysis::lemon::DirectedGraph::SubGraph componentGraph = gv.identifyConnectedComponents(graph);
        int componentNumber = ::lemon::countNodes( componentGraph.raw() );
        BOOST_REQUIRE_MESSAGE( componentNumber == 2, "Subgraph with '" << componentNumber << "' nodes representing components, while base graph has '" << ::lemon::countNodes( graph.raw()) << "' nodes overall" );
    }

    io::GraphIO::write("testfile-lemon", graph, representation::GEXF);
}

BOOST_AUTO_TEST_CASE(it_should_work_for_snap)
{
    ::graph_analysis::snap::DirectedGraph graph;

    Vertex::Ptr v0( new Vertex());
    Vertex::Ptr v1( new Vertex());

    BOOST_REQUIRE_THROW( v0->getId(graph.getId()), std::runtime_error);
    BOOST_REQUIRE_THROW( v1->getId(graph.getId()), std::runtime_error);

    graph.addVertex(v0);
    graph.addVertex(v1);

    BOOST_REQUIRE_NO_THROW(v0->getId(graph.getId()));
    BOOST_REQUIRE_NO_THROW(v1->getId(graph.getId()));

    Edge::Ptr e0(new Edge());
    BOOST_REQUIRE_THROW( e0->getId(graph.getId()), std::runtime_error);
    BOOST_REQUIRE_THROW(graph.addEdge(e0), std::runtime_error);

    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);
    BOOST_REQUIRE_NO_THROW(graph.addEdge(e0));

    VertexIterator::Ptr nodeIt = graph.getVertexIterator();
    while(nodeIt->next())
    {
        Vertex::Ptr vertex0 = nodeIt->current();
        BOOST_REQUIRE_MESSAGE( vertex0->toString() != "", "Vertex: " << vertex0->toString() );
    }

    EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge0 = edgeIt->current();
        BOOST_REQUIRE_MESSAGE( edge0->toString() != "", "Edge: " << edge0->toString() );
    }

    io::GraphIO::write("testfile-snap", graph, representation::GEXF);
}
