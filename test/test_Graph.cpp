#include <boost/test/unit_test.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/snap/Graph.hpp>
#include <graph_analysis/filters/CommonFilters.hpp>
#include <graph_analysis/BipartiteGraph.hpp>

#include <graph_analysis/GraphIO.hpp>

using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(graph_impl)

BOOST_AUTO_TEST_CASE(add_remove_edges_and_vertices)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        {
            BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
            BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

            Vertex::Ptr v0( new Vertex());
            Vertex::Ptr v1( new Vertex());

            Edge::Ptr e0(new Edge());
            e0->setSourceVertex(v0);
            e0->setTargetVertex(v1);

            BOOST_REQUIRE_NO_THROW(graph->addEdge(e0));
            BOOST_REQUIRE_THROW(graph->addVertex(v0), std::runtime_error);
            BOOST_REQUIRE_THROW(graph->addVertex(v1), std::runtime_error);

            int vertexCount = graph->getVertexCount();
            BOOST_REQUIRE_MESSAGE(graph->getVertexCount() == 2, "Count nodes: expected 2 vertices but was " << vertexCount);
            BOOST_REQUIRE_MESSAGE(graph->getEdgeCount() == 1, "Count edges: expected 1 edge but was " << vertexCount);

            size_t order = graph->order();
            BOOST_REQUIRE_MESSAGE(order == 2, "Graph order: expected order 2, but was " << order);

            size_t size = graph->size();
            BOOST_REQUIRE_MESSAGE(size == 1, "Graph size: expected size 1, but was " << size);

            BOOST_REQUIRE_NO_THROW(graph->removeEdge(e0));
            BOOST_REQUIRE_NO_THROW(graph->removeVertex(v0));
            BOOST_REQUIRE_NO_THROW(graph->removeVertex(v1));

            vertexCount = graph->getVertexCount();
            BOOST_REQUIRE_MESSAGE(vertexCount == 0, "Removed vertices: expected 0 but was " << vertexCount);
            BOOST_REQUIRE_MESSAGE(graph->getVertexCount() == 0, "Removed vertices: expected 0 but was " << vertexCount);
        }
        {
            BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
            for(int i = 0; i < 10; ++i)
            {
                graph->addVertex( Vertex::Ptr( new Vertex() ) );
            }
            BaseGraph::Ptr clonedGraph = graph->clone();

            std::vector<Vertex::Ptr> vertices = clonedGraph->getAllVertices();
            std::vector<Vertex::Ptr>::const_iterator cit = vertices.begin();
            for(; cit != vertices.end(); ++cit)
            {
                clonedGraph->removeVertex( *cit );
            }
        }
    }
}

BOOST_AUTO_TEST_CASE(clone)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());

        Edge::Ptr e0(new Edge());
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        BOOST_REQUIRE_NO_THROW(graph->addEdge(e0));
        BOOST_REQUIRE_THROW(graph->addVertex(v0), std::runtime_error);
        BOOST_REQUIRE_THROW(graph->addVertex(v1), std::runtime_error);

        BaseGraph::Ptr graph_clone = graph->clone();

        BOOST_REQUIRE_MESSAGE(graph->getVertexCount() == graph_clone->getVertexCount(), "Graph and clone have same number of vertices");
        BOOST_REQUIRE_MESSAGE(graph->getEdgeCount() == graph_clone->getEdgeCount(), "Graph and clone have same number of edges");

        VertexIterator::Ptr vertexIterator = graph->getVertexIterator();
        while(vertexIterator->next())
        {
            Vertex::Ptr vertex = vertexIterator->current();
            BOOST_REQUIRE_MESSAGE( !graph_clone->contains(vertex), "Graph clone does not contain vertex of originating graph");
        }

        EdgeIterator::Ptr edgeIterator = graph->getEdgeIterator();
        while(edgeIterator->next())
        {
            Edge::Ptr edge = edgeIterator->current();
            BOOST_REQUIRE_MESSAGE(!graph_clone->contains(edge), "Graph clone does not contain edge of originating graph");
        }
    }
}

BOOST_AUTO_TEST_CASE(clone_edges)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());

        Edge::Ptr e0(new Edge());
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        BOOST_REQUIRE_NO_THROW(graph->addEdge(e0));
        BOOST_REQUIRE_THROW(graph->addVertex(v0), std::runtime_error);
        BOOST_REQUIRE_THROW(graph->addVertex(v1), std::runtime_error);

        BaseGraph::Ptr graph_clone = graph->cloneEdges();

        BOOST_REQUIRE_MESSAGE(graph->getVertexCount() == graph_clone->getVertexCount(), "Graph and clone have same number of vertices");
        BOOST_REQUIRE_MESSAGE(graph->getEdgeCount() == graph_clone->getEdgeCount(), "Graph and clone have same number of edges");

        VertexIterator::Ptr vertexIterator = graph->getVertexIterator();
        while(vertexIterator->next())
        {
            Vertex::Ptr vertex = vertexIterator->current();
            BOOST_REQUIRE_MESSAGE( graph_clone->contains(vertex), "Graph clone does not contain vertex of originating graph");
        }

        EdgeIterator::Ptr edgeIterator = graph->getEdgeIterator();
        while(edgeIterator->next())
        {
            Edge::Ptr edge = edgeIterator->current();
            BOOST_REQUIRE_MESSAGE(!graph_clone->contains(edge), "Graph clone does not contain edge of originating graph");
        }
    }
}

BOOST_AUTO_TEST_CASE(iterator_over_edges_and_vertices)
{
    using namespace graph_analysis;

    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());

        Edge::Ptr e0(new Edge());
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        Edge::Ptr e1(new Edge());
        e1->setSourceVertex(v1);
        e1->setTargetVertex(v0);

        graph->addEdge(e0);
        graph->addEdge(e1);

        {
            EdgeIterator::Ptr edgeIt  = graph->getEdgeIterator(v0);
            int count = 0;
            while(edgeIt->next())
            {
                Edge::Ptr edge = edgeIt->current();
                count++;
            }
            BOOST_REQUIRE_MESSAGE(count == 2, "In and outgoing edges should sum up to 2");
        }
        {
            EdgeIterator::Ptr edgeIt  = graph->getEdgeIterator(v1);
            int count = 0;
            while(edgeIt->next())
            {
                Edge::Ptr edge = edgeIt->current();
                count++;
            }
            BOOST_REQUIRE_MESSAGE(count == 2, "In and outgoing edges should sum up to 2");
        }
    }
}

BOOST_AUTO_TEST_CASE(subgraph)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());
        Vertex::Ptr v2( new Vertex());
        Vertex::Ptr v3( new Vertex());

        BOOST_REQUIRE_THROW( v0->getId(graph->getId()), std::runtime_error);
        BOOST_REQUIRE_THROW( v1->getId(graph->getId()), std::runtime_error);
        BOOST_REQUIRE_THROW( v2->getId(graph->getId()), std::runtime_error);
        BOOST_REQUIRE_THROW( v3->getId(graph->getId()), std::runtime_error);

        graph->addVertex(v0);
        graph->addVertex(v1);
        graph->addVertex(v2);
        graph->addVertex(v3);

        BOOST_REQUIRE_NO_THROW(v0->getId(graph->getId()));
        BOOST_REQUIRE_NO_THROW(v1->getId(graph->getId()));
        BOOST_REQUIRE_NO_THROW(v2->getId(graph->getId()));
        BOOST_REQUIRE_NO_THROW(v3->getId(graph->getId()));

        Edge::Ptr e0(new Edge());
        BOOST_REQUIRE_THROW( e0->getId(graph->getId()), std::runtime_error);
        BOOST_REQUIRE_THROW(graph->addEdge(e0), std::runtime_error);
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);
        BOOST_REQUIRE_NO_THROW(graph->addEdge(e0));
        BOOST_REQUIRE_MESSAGE( e0->getSourceVertex()->getUid() != e0->getTargetVertex()->getUid(), "Edge should not point to same node");

        Edge::Ptr e1(new Edge());
        BOOST_REQUIRE_THROW( e1->getId(graph->getId()), std::runtime_error);
        BOOST_REQUIRE_THROW(graph->addEdge(e1), std::runtime_error);
        e1->setSourceVertex(v2);
        e1->setTargetVertex(v3);
        BOOST_REQUIRE_NO_THROW(graph->addEdge(e1));
        BOOST_REQUIRE_MESSAGE( e1->getSourceVertex()->getUid() != e1->getTargetVertex()->getUid(), "Edge should not point to same node");

        int graphCount = graph->getVertexCount();
        BOOST_REQUIRE_MESSAGE( graphCount == 4, "Graph contains all nodes");

        BaseGraph::Ptr copiedGraph = graph->copy();
        int copiedGraphCount = copiedGraph->getVertexCount();
        BOOST_REQUIRE_MESSAGE( graphCount == copiedGraphCount, "Graph copy contains all nodes " << copiedGraphCount << " of " << graphCount);

        VertexIterator::Ptr nodeIt = graph->getVertexIterator();
        while(nodeIt->next())
        {
            Vertex::Ptr vertex0 = nodeIt->current();
            BOOST_REQUIRE_MESSAGE( vertex0->toString() != "", "Vertex: " << vertex0->toString() );
        }

        EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
        while(edgeIt->next())
        {
            Edge::Ptr edge0 = edgeIt->current();
            BOOST_REQUIRE_MESSAGE( edge0->toString() != "", "Edge: " << edge0->toString() );
        }

        {
            BaseGraph::Ptr baseGraph = graph->newInstance();
            baseGraph->addEdge(e0);
            baseGraph->addEdge(e1);

            //graph_analysis::Filter< Vertex::Ptr >::Ptr vertexFilter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
            //graph_analysis::Filter< Edge::Ptr >::Ptr edgeFilter(new graph_analysis::filters::PermitAll< Edge::Ptr >() );
            //subgraph->applyFilters(vertexFilter, edgeFilter);

            try {
                graph_analysis::SubGraph::Ptr subgraph = baseGraph->identifyConnectedComponents(baseGraph);

                int componentNumber = subgraph->getVertexCount();
                BOOST_REQUIRE_MESSAGE( componentNumber == 2, "Subgraph with '" << componentNumber << "' vertices representing components, while base graph has '" << baseGraph->getVertexCount() << "' vertices overall" );
            } catch(const std::runtime_error& e)
            {
                BOOST_TEST_MESSAGE(e.what() << " -- for " << baseGraph->getImplementationTypeName());
            }

        }
        {
            BaseGraph::Ptr baseGraph = graph->newInstance();
            baseGraph->addEdge(e0);
            baseGraph->addEdge(e1);

            graph_analysis::SubGraph::Ptr subgraph = BaseGraph::getSubGraph(baseGraph);

            graph_analysis::Filter< Vertex::Ptr >::Ptr vertexFilter(new graph_analysis::filters::PermitAll< Vertex::Ptr >() );
            graph_analysis::Filter< Edge::Ptr >::Ptr edgeFilter(new graph_analysis::filters::PermitAll< Edge::Ptr >() );
            subgraph->applyFilters(vertexFilter, edgeFilter);

            BaseGraph::Ptr baseGraphCopy = subgraph->toBaseGraph();


            try {
                subgraph = baseGraphCopy->identifyConnectedComponents(baseGraphCopy);

                int componentNumber = subgraph->getVertexCount();
                BOOST_REQUIRE_MESSAGE( componentNumber == 2, "Subgraph with '" << componentNumber << "' vertices representing components, while base graph has '" << baseGraphCopy->getVertexCount() << "' vertices overall" );
            } catch(const std::runtime_error& e)
            {
                BOOST_TEST_MESSAGE(e.what() << " -- for " << baseGraph->getImplementationTypeName());
            }

        }
    }
}

BOOST_AUTO_TEST_CASE(get_edges)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        Vertex::Ptr v0( new Vertex());
        v0->setLabel("v0");
        Vertex::Ptr v1( new Vertex());
        v1->setLabel("v1");

        Edge::Ptr e0(new Edge());
        e0->setLabel("v0->v1");
        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);

        Edge::Ptr e1(new Edge());
        e1->setLabel("v1->v0");
        e1->setSourceVertex(v1);
        e1->setTargetVertex(v0);

        graph->addEdge(e0);
        graph->addEdge(e1);


        {
            std::vector<Edge::Ptr> edges = graph->getEdges(v0,v1);
            BOOST_REQUIRE_MESSAGE(edges.size() == 1, "Get edges expected to return 1 edge, was " << edges.size() <<
                    " for " << graph->getImplementationTypeName());
        }
        {
            std::vector<Edge::Ptr> edges = graph->getEdges(v1,v0);
            BOOST_REQUIRE_MESSAGE(edges.size() == 1, "Get edges expected to return 1 edge for reverse direction, was " << edges.size() <<
                    " for " << graph->getImplementationTypeName());
        }
    }
}

BOOST_AUTO_TEST_CASE(bipartite_graph)
{
    BipartiteGraph bipartiteGraph;
    std::vector<Vertex::Ptr> reds;
    std::vector<Vertex::Ptr> blues;

    for(int i = 0; i < 100; ++i)
    {
        std::stringstream ss;
        ss << i;
        Vertex::Ptr blue( new Vertex(ss.str()));
        Vertex::Ptr red( new Vertex(ss.str()));

        blues.push_back(blue);
        reds.push_back(red);

        bipartiteGraph.linkVertices(red, blue);
    }

    for(int i = 0; i < 100; ++i)
    {

        BOOST_REQUIRE_MESSAGE(bipartiteGraph.getRedSubGraph()->enabled(reds[i]), "Red vertex is enabled in red subgraph");
        BOOST_REQUIRE_MESSAGE(bipartiteGraph.getBlueSubGraph()->enabled(blues[i]), "Red vertex is enabled in blue subgraph");

        BOOST_REQUIRE_MESSAGE(!bipartiteGraph.getRedSubGraph()->enabled(blues[i]), "Red vertex is disabled in red subgraph");
        BOOST_REQUIRE_MESSAGE(!bipartiteGraph.getBlueSubGraph()->enabled(reds[i]), "Red vertex is disabled in blue subgraph");

        std::vector<Vertex::Ptr> red = bipartiteGraph.getPartners(blues[i]);
        BOOST_REQUIRE(red.size() == 1);
        BOOST_REQUIRE(red[0] == reds[i]);

        std::vector<Vertex::Ptr> blue = bipartiteGraph.getPartners(reds[i]);
        BOOST_REQUIRE(blue.size() == 1);
        BOOST_REQUIRE(blue[0] == blues[i]);

        BOOST_REQUIRE(reds[i]  == bipartiteGraph.getUniquePartner(blues[i]));
        BOOST_REQUIRE(blues[i] == bipartiteGraph.getUniquePartner(reds[i]));
    }

}

BOOST_AUTO_TEST_SUITE_END()
