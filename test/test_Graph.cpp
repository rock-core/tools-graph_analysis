#include <boost/test/unit_test.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/CommonFilters.hpp>

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

    graph.write();

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
        graph_analysis::GraphView< ::graph_analysis::lemon::DirectedGraph,Vertex::Ptr, Edge::Ptr> gv;
        graph_analysis::Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filter::PermitAll< Vertex::Ptr >() );
        gv.setVertexFilter(filter);

        ::graph_analysis::lemon::DirectedSubGraph subGraph = gv.apply(graph);
        int subgraphCount = ::lemon::countNodes(subGraph.raw());
        int graphCount = ::lemon::countNodes(graph.raw());
        BOOST_REQUIRE_MESSAGE( subgraphCount == graphCount, "Subgraph contains all nodes after applying PermitAll filter: " << subgraphCount << " vs. " << graphCount);
    }

    {
        graph_analysis::GraphView< ::graph_analysis::lemon::DirectedGraph, Vertex::Ptr, Edge::Ptr> gv;
        graph_analysis::Filter< Vertex::Ptr >::Ptr filter(new graph_analysis::filter::DenyAll< Vertex::Ptr >() );
        gv.setVertexFilter(filter);

        ::graph_analysis::lemon::DirectedSubGraph subGraph = gv.apply(graph);

        int subgraphCount = ::lemon::countNodes(subGraph.raw());
        BOOST_REQUIRE_MESSAGE( subgraphCount == 0, "Subgraph contains no nodes after applying DenyAll filter" << subgraphCount << " vs. 0 " );
    }
}


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
