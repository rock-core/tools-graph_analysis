#include <boost/test/unit_test.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>
#include <graph_analysis/WeightedVertex.hpp>
#include <graph_analysis/WeightedEdge.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <graph_analysis/io/GVGraph.hpp>

using namespace graph_analysis;

class SimpleStyle : public io::GraphvizStyle
{
public:
    void apply(const Edge::Ptr& edge, io::GVGraph* gvGraph, const BaseGraph::Ptr& graph) const
    {
        gvGraph->setAttribute(edge, "color", "red");
        gvGraph->setAttribute(edge, "weight", "10");
        gvGraph->setAttribute(edge, "arrowsize", "2");
        gvGraph->setAttribute(edge, "penwidth", "3");
    }

    void apply(const Vertex::Ptr& vertex, io::GVGraph* gvGraph, const BaseGraph::Ptr& graph) const
    {
        gvGraph->setAttribute(vertex, "color", "blue");
        gvGraph->setAttribute(vertex, "shape", "diamond");
        gvGraph->setAttribute(vertex, "penwidth", "3");
    }
};

BOOST_AUTO_TEST_SUITE(graph_io)
/*
 * Note:
 * ----
 * These tests imply that GraphvizWriter class works correctly - it constructs a complete graph with 4 nodes
 * and renders it to file of hardwired name "testgraph->dot"
 * (manual dot rendering of the output .dot file and checking it is assumed)
 */
class DerivedVertex : public graph_analysis::Vertex
{
public:
    DerivedVertex(std::string name)
        : graph_analysis::Vertex(name)
    {}

    std::string serializeMember0() { return mMember0; }
    std::string serializeMember1() { return mMember1; }

    void deserializeMember0(const std::string& s) { mMember0= s; }
    void deserializeMember1(const std::string& s) { mMember1 = s; }

    virtual std::string getClassName() const{ return "DerivedVertex"; }

    std::string mMember0;
    std::string mMember1;
protected:
    virtual graph_analysis::Vertex* getClone() const { return new DerivedVertex("CLONE"); }

};

class DerivedEdge : public graph_analysis::Edge
{
public:
    DerivedEdge(std::string name)
        : graph_analysis::Edge(name)
    {}

    std::string serializeMember0() { return mMember0; }
    std::string serializeMember1() { return mMember1; }

    void deserializeMember0(const std::string& s) { mMember0= s; }
    void deserializeMember1(const std::string& s) { mMember1 = s; }

    virtual std::string getClassName() const{ return "DerivedEdge"; }

    std::string mMember0;
    std::string mMember1;
protected:
    virtual graph_analysis::Edge* getClone() const { return new DerivedEdge("CLONE"); }

};

BOOST_AUTO_TEST_CASE(dot)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        BOOST_REQUIRE_MESSAGE(true, "constructing test graph");
        BOOST_REQUIRE_MESSAGE(true, "    constructing 4 vertices");
        Vertex::Ptr v0(new Vertex("label 0"));
        Vertex::Ptr v1(new Vertex("label 1"));
        Vertex::Ptr v2(new Vertex("label 2"));
        Vertex::Ptr v3(new Vertex("label 3"));

        BOOST_REQUIRE_MESSAGE(true, "    constructing 6 edges");
        Edge::Ptr e0(new Edge(v0, v1, "edge label 0"));
        Edge::Ptr e1(new Edge(v0, v2));
        Edge::Ptr e2(new Edge(v0, v3));
        Edge::Ptr e3(new Edge(v1, v2));
        Edge::Ptr e4(new Edge(v1, v3));
        Edge::Ptr e5(new Edge(v2, v3));
        // Add multiedge
        Edge::Ptr e6(new Edge(v0, v1, "edge label 1"));

        BOOST_REQUIRE_MESSAGE(true, "    adding the 4 vertices to graph");
        BOOST_TEST_MESSAGE("Added vertex with id: " << graph->addVertex(v0));
        BOOST_TEST_MESSAGE("Added vertex with id: " << graph->addVertex(v1));
        BOOST_TEST_MESSAGE("Added vertex with id: " << graph->addVertex(v2));
        BOOST_TEST_MESSAGE("Added vertex with id: " << graph->addVertex(v3));

        BOOST_REQUIRE_MESSAGE(true, "    adding the 6 edges to graph");
        graph->addEdge(e0);
        graph->addEdge(e1);
        graph->addEdge(e2);
        graph->addEdge(e3);
        graph->addEdge(e4);
        graph->addEdge(e5);
        graph->addEdge(e6);
        BOOST_REQUIRE_MESSAGE(true, "constructed test graph successfully");

        std::string filename = "/tmp/test-" + graph->getImplementationTypeName() + "-graphviz.dot";
        BOOST_REQUIRE_MESSAGE(true, "Starting rendering to file " << filename);
        io::GraphIO::write(filename, *graph, representation::GRAPHVIZ);
        BOOST_REQUIRE_MESSAGE(true, "Rendering completed. Please manually check a complete graph with 4-nodes is contained in file " << filename);
    }
}

BOOST_AUTO_TEST_CASE(gexf)
{
    using namespace graph_analysis;

    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        Vertex::Ptr v0( new Vertex());
        Vertex::Ptr v1( new Vertex());

        BOOST_REQUIRE_THROW( v0->getId(graph->getId()), std::runtime_error);
        BOOST_REQUIRE_THROW( v1->getId(graph->getId()), std::runtime_error);

        graph->addVertex(v0);
        graph->addVertex(v1);

        BOOST_REQUIRE_NO_THROW(v0->getId(graph->getId()));
        BOOST_REQUIRE_NO_THROW(v1->getId(graph->getId()));

        Edge::Ptr e0(new Edge());
        BOOST_REQUIRE_THROW( e0->getId(graph->getId()), std::runtime_error);
        BOOST_REQUIRE_THROW(graph->addEdge(e0), std::runtime_error);

        e0->setSourceVertex(v0);
        e0->setTargetVertex(v1);
        BOOST_REQUIRE_NO_THROW(graph->addEdge(e0));

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

        std::string filename = "/tmp/test-io-" + graph->getImplementationTypeName() + ".gexf";
        io::GraphIO::write(filename, *graph, representation::GEXF);
    }
}

BOOST_AUTO_TEST_CASE(dot_with_derived_types)
{
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

        BOOST_REQUIRE_MESSAGE(true, "constructing test graph");
        BOOST_REQUIRE_MESSAGE(true, "    constructing 4 vertices");
        WeightedVertex::Ptr v0(new WeightedVertex(10.0));
        WeightedVertex::Ptr v1(new WeightedVertex(15.0));
        WeightedEdge::Ptr e0(new WeightedEdge(v0, v1, 20.0));

        graph->addEdge(e0);

        std::string filename = "/tmp/test-" + graph->getImplementationTypeName() + "-graphviz-with-derived-types.dot";
        BOOST_REQUIRE_MESSAGE(true, "Starting rendering to file " << filename);
        io::GraphIO::write(filename, *graph, representation::GRAPHVIZ);
        BOOST_REQUIRE_MESSAGE(true, "Rendering completed. Please manually check a complete graph with 4-nodes is contained in file " << filename);
    }
}


BOOST_AUTO_TEST_CASE(gexf_derived_type_and_members)
{
    using namespace graph_analysis;

    BaseGraph::Ptr graph = BaseGraph::getInstance(BaseGraph::LEMON_DIRECTED_GRAPH);
    BOOST_TEST_MESSAGE("BaseGraph implementation: " << graph->getImplementationTypeName());

    Vertex::Ptr empty( new DerivedVertex("empty"));
    VertexTypeManager *vManager = VertexTypeManager::getInstance();

    vManager->registerType(empty);
    vManager->registerAttribute(empty->getClassName(), "m0",
            (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedVertex::serializeMember0,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedVertex::deserializeMember0,
            (io::AttributeSerializationCallbacks::print_func_t)&DerivedVertex::serializeMember0);

    vManager->registerAttribute(empty->getClassName(), "m1",
            (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedVertex::serializeMember1,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedVertex::deserializeMember1,
            (io::AttributeSerializationCallbacks::print_func_t) &DerivedVertex::serializeMember1);

    Vertex::Ptr v0( new DerivedVertex("v1"));
    Vertex::Ptr v1( new DerivedVertex("v2"));
    DerivedVertex *orginalSource = dynamic_cast<DerivedVertex*>(v0.get());
    DerivedVertex *orginalTarget= dynamic_cast<DerivedVertex*>(v1.get());

    orginalSource->mMember0="v1-m0";
    orginalTarget->mMember0="v2-m0";
    orginalSource->mMember1="v1-m1";
    orginalTarget->mMember1="v2-m1";

    graph->addVertex(v0);
    graph->addVertex(v1);


    Edge::Ptr e0(new DerivedEdge("derived-edge"));
    EdgeTypeManager* eManager = EdgeTypeManager::getInstance();
    eManager->registerType(e0);
    eManager->registerAttribute(e0->getClassName(), "a0",
            (io::AttributeSerializationCallbacks::serialize_func_t) &DerivedEdge::serializeMember0,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedEdge::deserializeMember0,
            (io::AttributeSerializationCallbacks::print_func_t)&DerivedEdge::serializeMember0);

    eManager->registerAttribute(e0->getClassName(), "a1",
            (io::AttributeSerializationCallbacks::serialize_func_t) &DerivedEdge::serializeMember1,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedEdge::deserializeMember1,
            (io::AttributeSerializationCallbacks::print_func_t)&DerivedEdge::serializeMember1);

    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    DerivedEdge* originalEdge = dynamic_cast<DerivedEdge*>( e0.get () );
    originalEdge->mMember0 ="e0-a0";
    originalEdge->mMember1 ="e0-a1";

    graph->addEdge(e0);

    std::cout << "Write graph" << std::endl;
    std::string filename = "/tmp/test-io-" + graph->getImplementationTypeName() + ".gexf";
    io::GraphIO::write(filename, graph, representation::GEXF);

    std::cout << "Read graph" << std::endl;
    BaseGraph::Ptr read_graph = BaseGraph::getInstance(graph->getImplementationType());
    io::GraphIO::read(filename, read_graph, representation::GEXF);

    BOOST_REQUIRE_MESSAGE(read_graph->size() == 1, "Read graph has wrong size ");


    EdgeIterator::Ptr edgeIt = read_graph->getEdgeIterator();
    int count=0;
    while(edgeIt->next())
    {
        count++;

        Edge::Ptr edge0 = edgeIt->current();
        BOOST_REQUIRE_MESSAGE( edge0->getTargetVertex()->getClassName() == v0->getClassName(), "Imported Class name is invalid" );
        DerivedEdge* importedEdge = dynamic_cast<DerivedEdge*>(edge0.get());
        BOOST_REQUIRE_MESSAGE( importedEdge->mMember0 == originalEdge->mMember0, "Edge: member0 was imported wrongly");
        BOOST_REQUIRE_MESSAGE( importedEdge->mMember1 == originalEdge->mMember1, "Edge: member1 was imported wrongly");


        BOOST_REQUIRE_MESSAGE( edge0->getSourceVertex()->getClassName() == v1->getClassName(), "Imported Class name is invalid" );
        DerivedVertex *importedSource = dynamic_cast<DerivedVertex*>(edge0->getSourceVertex().get());
        DerivedVertex *importedTarget= dynamic_cast<DerivedVertex*>(edge0->getTargetVertex().get());


        BOOST_REQUIRE_MESSAGE(importedSource, "Imported type of source is incorrect" );
        BOOST_REQUIRE_MESSAGE(importedTarget, "Imported type of target is incorrect" );

        BOOST_REQUIRE_MESSAGE( importedSource->mMember0 == orginalSource->mMember0, "Member0 was imported wrongly" );
        BOOST_REQUIRE_MESSAGE( importedSource->mMember1 == orginalSource->mMember1, "Member1 was imported wrongly" );
        BOOST_REQUIRE_MESSAGE( importedTarget->mMember0 == orginalTarget->mMember0, "Member0 was imported wrongly" );
        BOOST_REQUIRE_MESSAGE( importedTarget->mMember1 == orginalTarget->mMember1, "Member1 was imported wrongly" );

    }
    BOOST_REQUIRE_MESSAGE( count == 1, "Iterator in graph failed");
}

BOOST_AUTO_TEST_CASE(graphviz_css)
{
    BaseGraph::Ptr graph = BaseGraph::getInstance();

    Vertex::Ptr v0(new Vertex("v0"));
    Vertex::Ptr v1(new Vertex("v1"));
    Vertex::Ptr v2(new Vertex("v2"));

    Edge::Ptr e0(new Edge(v0,v1,"e0"));
    Edge::Ptr e1(new Edge(v1,v2,"e1"));
    Edge::Ptr e2(new Edge(v2,v0,"e2"));

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);

    io::GraphvizWriter gvWriter;
    io::GraphvizStyle::Ptr style(new SimpleStyle());
    gvWriter.setStyle(style);

    gvWriter.write("/tmp/graph_analysis-test-graph_io-graphviz_css-simple.dot", graph);
}

BOOST_AUTO_TEST_SUITE_END()
