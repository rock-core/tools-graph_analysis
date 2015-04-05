#include <boost/test/unit_test.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/snap/Graph.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/filters/CommonFilters.hpp>

#include <graph_analysis/GraphIO.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(lemon)

BOOST_AUTO_TEST_CASE(it_should_add_remove_edges_and_vertices)
{
    graph_analysis::lemon::DirectedGraph graph;

    Vertex::Ptr v0( new Vertex());
    Vertex::Ptr v1( new Vertex());

    Edge::Ptr e0(new Edge());
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    BOOST_REQUIRE_NO_THROW(graph.addEdge(e0));
    BOOST_REQUIRE_THROW(graph.addVertex(v0), std::runtime_error);
    BOOST_REQUIRE_THROW(graph.addVertex(v1), std::runtime_error);

    int vertexCount = ::lemon::countNodes(graph.raw());
    BOOST_REQUIRE_MESSAGE(vertexCount == 2, "Auto adding of vertices via edges: expected 2 vertices but was " << vertexCount);
    BOOST_REQUIRE_MESSAGE(graph.getVertexCount() == 2, "Count nodes: expected 2 vertices but was " << vertexCount);
    BOOST_REQUIRE_MESSAGE(graph.getEdgeCount() == 1, "Count edges: expected 1 edge but was " << vertexCount);

    BOOST_REQUIRE_NO_THROW(graph.removeEdge(e0));
    BOOST_REQUIRE_NO_THROW(graph.removeVertex(v0));
    BOOST_REQUIRE_NO_THROW(graph.removeVertex(v1));

    vertexCount = ::lemon::countNodes(graph.raw());
    BOOST_REQUIRE_MESSAGE(vertexCount == 0, "Removed vertices: expected 0 but was " << vertexCount);
    BOOST_REQUIRE_MESSAGE(graph.getVertexCount() == 0, "Removed vertices: expected 0 but was " << vertexCount);
}

BOOST_AUTO_TEST_CASE(it_should_iterate_over_vertices_and_edges)
{
    using namespace graph_analysis;

    graph_analysis::lemon::DirectedGraph graph;

    Vertex::Ptr v0( new Vertex());
    Vertex::Ptr v1( new Vertex());

    Edge::Ptr e0(new Edge());
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    Edge::Ptr e1(new Edge());
    e1->setSourceVertex(v1);
    e1->setTargetVertex(v0);

    graph.addEdge(e0);
    graph.addEdge(e1);

    {
        EdgeIterator::Ptr edgeIt  = graph.getEdgeIterator(v0);
        int count = 0;
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            count++;
        }
        BOOST_REQUIRE_MESSAGE(count == 2, "In and outgoing edges should sum up to 2");
    }
    {
        EdgeIterator::Ptr edgeIt  = graph.getEdgeIterator(v1);
        int count = 0;
        while(edgeIt->next())
        {
            Edge::Ptr edge = edgeIt->current();
            count++;
        }
        BOOST_REQUIRE_MESSAGE(count == 2, "In and outgoing edges should sum up to 2");
    }

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
        BaseGraph::Ptr baseGraph = graph.copy();
        graph_analysis::GraphView gv;
        graph_analysis::Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
        gv.setVertexFilter(filter);

        graph_analysis::SubGraph::Ptr subGraph = gv.apply(baseGraph);
        int subgraphCount = subGraph->getVertexCount();
        int graphCount = baseGraph->getVertexCount();
        BOOST_REQUIRE_MESSAGE( subgraphCount == graphCount, "Subgraph contains all nodes after applying PermitAll filter: '" << subgraphCount << "' vs. '" << graphCount << "'");
    }

    {
        BaseGraph::Ptr baseGraph = graph.copy();
        graph_analysis::GraphView gv;
        graph_analysis::Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filters::DenyAll< Vertex::Ptr >() );
        gv.setVertexFilter(filter);

        graph_analysis::SubGraph::Ptr subGraph = gv.apply(baseGraph);
        int subgraphCount = subGraph->getVertexCount();
        BOOST_REQUIRE_MESSAGE( subgraphCount == 0, "Subgraph contains no nodes after applying DenyAll filter '" << subgraphCount << "' expected '0'" );
    }
    {
        BaseGraph::Ptr baseGraph = graph.cleanCopy();
        baseGraph->addEdge(e0);
        baseGraph->addEdge(e1);

        //graph_analysis::Filter< Vertex::Ptr >::Ptr vertexFilter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
        //graph_analysis::Filter< Edge::Ptr >::Ptr edgeFilter(new graph_analysis::filters::PermitAll< Edge::Ptr >() );
        //subgraph->applyFilters(vertexFilter, edgeFilter);

        graph_analysis::SubGraph::Ptr subgraph = baseGraph->identifyConnectedComponents(baseGraph);

        int componentNumber = subgraph->getVertexCount();
        BOOST_REQUIRE_MESSAGE( componentNumber == 2, "Subgraph with '" << componentNumber << "' vertices representing components, while base graph has '" << baseGraph->getVertexCount() << "' vertices overall" );
    }
    {
        BaseGraph::Ptr baseGraph = graph.cleanCopy();
        baseGraph->addEdge(e0);
        baseGraph->addEdge(e1);

        graph_analysis::SubGraph::Ptr subgraph = BaseGraph::getSubGraph(baseGraph);

        graph_analysis::Filter< Vertex::Ptr >::Ptr vertexFilter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
        graph_analysis::Filter< Edge::Ptr >::Ptr edgeFilter(new graph_analysis::filters::PermitAll< Edge::Ptr >() );
        subgraph->applyFilters(vertexFilter, edgeFilter);

        BaseGraph::Ptr baseGraphCopy = subgraph->toBaseGraph();


        subgraph = baseGraphCopy->identifyConnectedComponents(baseGraphCopy);

        int componentNumber = subgraph->getVertexCount();
        BOOST_REQUIRE_MESSAGE( componentNumber == 2, "Subgraph with '" << componentNumber << "' vertices representing components, while base graph has '" << baseGraphCopy->getVertexCount() << "' vertices overall" );
    }

    io::GraphIO::write("testfile-lemon", graph, representation::GEXF);
}

BOOST_AUTO_TEST_SUITE_END()


BOOST_AUTO_TEST_SUITE(snap)

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

BOOST_AUTO_TEST_SUITE_END()
