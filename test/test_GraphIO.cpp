#include <boost/test/unit_test.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/io/GraphvizWriter.hpp>
#include <graph_analysis/WeightedVertex.hpp>
#include <graph_analysis/WeightedEdge.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include <graph_analysis/EdgeTypeManager.hpp>

#include <graph_analysis/io/GVGraph.hpp>
#include <graph_analysis/io/GraphvizGridStyle.hpp>
#include "test_utils.hpp"

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
    DerivedVertex(std::string name = "")
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
    DerivedEdge(std::string name = "")
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

class RowColumnVertex : public graph_analysis::Vertex
{
public:
    typedef shared_ptr<RowColumnVertex> Ptr;

    RowColumnVertex(const std::string& name, size_t row, size_t column)
        : graph_analysis::Vertex(name)
        , row(row)
        , column(column)
    {}

    virtual std::string getClassName() const{ return "RowColumnVertex"; }

    size_t row;
    size_t column;

    static size_t getRow(const graph_analysis::Vertex::Ptr& vertex)
    {
        return dynamic_pointer_cast<RowColumnVertex>(vertex)->row;
    }

    static size_t getColumn(const graph_analysis::Vertex::Ptr& vertex)
    {
        return dynamic_pointer_cast<RowColumnVertex>(vertex)->column;
    }

protected:
    virtual graph_analysis::Vertex* getClone() const { return new RowColumnVertex(getLabel(), row, column); }

};

class DerivedVertexA : public graph_analysis::Vertex
{
public:
    DerivedVertexA(const std::string& name = "")
        : graph_analysis::Vertex(name)
    {}

    virtual std::string serializeMember0() { return mMember0; }
    virtual std::string serializeMember1() { return mMember1; }

    virtual void deserializeMember0(const std::string& s) { mMember0 = s; }
    virtual void deserializeMember1(const std::string& s) { mMember1 = s; }

    virtual std::string getClassName() const{ return "DerivedVertexA"; }

    std::string mMember0;
    std::string mMember1;
protected:
    virtual graph_analysis::Vertex* getClone() const { return new DerivedVertexA("CLONE"); }

};

class DerivedVertexB : public DerivedVertexA
{
public:
    DerivedVertexB(const std::string& name = "")
        : DerivedVertexA(name)
    {}

    std::string serializeMember2() { return mMember2; }
    void deserializeMember2(const std::string& s) { mMember2 = s; }

    virtual std::string getClassName() const{ return "DerivedVertexB"; }

    std::string mMember2;
protected:
    virtual graph_analysis::Vertex* getClone() const { return new DerivedVertexB("CLONE"); }

};

class DerivedEdgeA : public graph_analysis::Edge
{
public:
    DerivedEdgeA(const std::string& name = "")
        : graph_analysis::Edge(name)
    {}

    virtual std::string serializeMember0() { return mMember0; }
    virtual std::string serializeMember1() { return mMember1; }

    virtual void deserializeMember0(const std::string& s) { mMember0 = s; }
    virtual void deserializeMember1(const std::string& s) { mMember1 = s; }

    virtual std::string getClassName() const{ return "DerivedEdgeA"; }

    std::string mMember0;
    std::string mMember1;
protected:
    virtual graph_analysis::Edge* getClone() const { return new DerivedEdgeA("CLONE"); }
};

class DerivedEdgeB : public DerivedEdgeA
{
public:
    DerivedEdgeB(const std::string& name = "")
        : DerivedEdgeA(name)
    {}

    virtual std::string serializeMember2() { return mMember2; }
    virtual void deserializeMember2(const std::string& s) { mMember2 = s; }

    virtual std::string getClassName() const{ return "DerivedEdgeB"; }

    std::string mMember2;
protected:
    virtual graph_analysis::Edge* getClone() const { return new DerivedEdgeB("CLONE"); }
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

BOOST_AUTO_TEST_CASE(gexf_multiedge)
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

        Edge::Ptr e1(new Edge());
        BOOST_REQUIRE_THROW( e1->getId(graph->getId()), std::runtime_error);
        BOOST_REQUIRE_THROW(graph->addEdge(e1), std::runtime_error);

        e1->setSourceVertex(v0);
        e1->setTargetVertex(v1);
        BOOST_REQUIRE_NO_THROW(graph->addEdge(e1));

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
        WeightedVertex::Ptr v0 = make_shared<WeightedVertex>(10.0);
        WeightedVertex::Ptr v1 = make_shared<WeightedVertex>(15.0);

        //BOOST_REQUIRE_MESSAGE( WeightedVertex::msRegistration.isRegistered(),
                //"weighted edge is reqistered");

        WeightedEdge::Ptr e0 = make_shared<WeightedEdge>(v0, v1, 20.0);
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

    Vertex::Ptr empty = make_shared<DerivedVertex>("empty");
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

    Vertex::Ptr v0 = make_shared<DerivedVertex>("v1");
    Vertex::Ptr v1 = make_shared<DerivedVertex>("v2");
    DerivedVertex *orginalSource = dynamic_cast<DerivedVertex*>(v0.get());
    DerivedVertex *orginalTarget= dynamic_cast<DerivedVertex*>(v1.get());

    orginalSource->mMember0="v1-m0";
    orginalTarget->mMember0="v2-m0";
    orginalSource->mMember1="v1-m1";
    orginalTarget->mMember1="v2-m1";

    graph->addVertex(v0);
    graph->addVertex(v1);


    Edge::Ptr e0 = make_shared<DerivedEdge>("derived-edge");
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

        BOOST_REQUIRE_MESSAGE( importedSource->mMember0 == orginalSource->mMember0, "Member0 was imported wrongly: "
                << importedSource->mMember0 << " vs. " << orginalSource->mMember0);
        BOOST_REQUIRE_MESSAGE( importedSource->mMember1 == orginalSource->mMember1, "Member1 was imported wrongly" );
        BOOST_REQUIRE_MESSAGE( importedTarget->mMember0 == orginalTarget->mMember0, "Member0 was imported wrongly" );
        BOOST_REQUIRE_MESSAGE( importedTarget->mMember1 == orginalTarget->mMember1, "Member1 was imported wrongly" );

    }
    BOOST_REQUIRE_MESSAGE( count == 1, "Iterator in graph failed");
}

BOOST_AUTO_TEST_CASE(derived_types)
{
    using namespace graph_analysis;

    BaseGraph::Ptr graph = BaseGraph::getInstance();
    VertexTypeManager *vManager = VertexTypeManager::getInstance();
    {
        Vertex::Ptr empty = make_shared<DerivedVertexA>("empty");


        vManager->registerType<DerivedVertexA>();
        vManager->registerAttribute(empty->getClassName(), "m0",
                (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedVertexA::serializeMember0,
                (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedVertexA::deserializeMember0,
                (io::AttributeSerializationCallbacks::print_func_t)&DerivedVertexA::serializeMember0);

        vManager->registerAttribute(empty->getClassName(), "m1",
                (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedVertexA::serializeMember1,
                (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedVertexA::deserializeMember1,
                (io::AttributeSerializationCallbacks::print_func_t) &DerivedVertexA::serializeMember1);
    }
    {
        Vertex::Ptr empty = make_shared<DerivedVertexB>("empty");

        vManager->registerType<DerivedVertexB, DerivedVertexA>();
        vManager->registerAttribute(empty->getClassName(), "m2",
                (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedVertexB::serializeMember2,
                (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedVertexB::deserializeMember2,
                (io::AttributeSerializationCallbacks::print_func_t) &DerivedVertexB::serializeMember2);
    }

    shared_ptr<DerivedVertexA> v0 = make_shared<DerivedVertexA>("v0");
    shared_ptr<DerivedVertexB> v1 = make_shared<DerivedVertexB>("v1");
    DerivedVertexA *orginalSource = dynamic_cast<DerivedVertexA*>(v0.get());
    DerivedVertexB *orginalTarget= dynamic_cast<DerivedVertexB*>(v1.get());

    orginalSource->mMember0="v0-m0";
    orginalSource->mMember1="v0-m1";

    orginalTarget->mMember0="v1-m0";
    orginalTarget->mMember1="v1-m1";
    orginalTarget->mMember2="v1-m2";

    graph->addVertex(v0);
    graph->addVertex(v1);

    EdgeTypeManager *eManager = EdgeTypeManager::getInstance();
    {
        Edge::Ptr empty = make_shared<DerivedEdgeA>("emptyEdgeA");
        eManager->registerType<DerivedEdgeA>();
        eManager->registerAttribute(empty->getClassName(), "m0",
                (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedEdgeA::serializeMember0,
                (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedEdgeA::deserializeMember0,
                (io::AttributeSerializationCallbacks::print_func_t)&DerivedEdgeA::serializeMember0);

        eManager->registerAttribute(empty->getClassName(), "m1",
                (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedEdgeA::serializeMember1,
                (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedEdgeA::deserializeMember1,
                (io::AttributeSerializationCallbacks::print_func_t) &DerivedEdgeA::serializeMember1);
    }
    {
        Edge::Ptr empty = make_shared<DerivedEdgeB>("emptyEdgeB");
        eManager->registerType<DerivedEdgeB, DerivedEdgeA>();
        eManager->registerAttribute(empty->getClassName(), "m2",
                (io::AttributeSerializationCallbacks::serialize_func_t)&DerivedEdgeB::serializeMember2,
                (io::AttributeSerializationCallbacks::deserialize_func_t)&DerivedEdgeB::deserializeMember2,
                (io::AttributeSerializationCallbacks::print_func_t) &DerivedEdgeB::serializeMember2);
    }

    shared_ptr<DerivedEdgeA> edgeA = make_shared<DerivedEdgeA>("e_A");
    shared_ptr<DerivedEdgeB> edgeB = make_shared<DerivedEdgeB>("e_B");

    edgeA->mMember0 = "e_A-m0";
    edgeA->mMember1 = "e_A-m1";
    edgeA->setTargetVertex(v0),
    edgeA->setSourceVertex(v1),

    edgeB->mMember0 = "e_B-m0";
    edgeB->mMember1 = "e_B-m1";
    edgeB->mMember2 = "e_B-m2";
    edgeB->setTargetVertex(v1),
    edgeB->setSourceVertex(v0),

    graph->addEdge(edgeA);
    graph->addEdge(edgeB);

    std::string filename = "/tmp/test-io-derived.gexf";
    io::GraphIO::write(filename, graph, representation::GEXF);

    BaseGraph::Ptr read_graph = BaseGraph::getInstance();
    io::GraphIO::read(filename, read_graph, representation::GEXF);

    VertexIterator::Ptr vertexIt = read_graph->getVertexIterator();
    while(vertexIt->next())
    {
        Vertex::Ptr vertex = vertexIt->current();
        if(vertex->getLabel() ==  "v0")
        {
            DerivedVertexA* v = dynamic_cast<DerivedVertexA*>(vertex.get());
            BOOST_REQUIRE_MESSAGE(v->mMember0 == v0->mMember0,
                    "Vertex 0 member 0 has been set correctly");
            BOOST_REQUIRE_MESSAGE(v->mMember1 == v0->mMember1,
                    "Vertex 0 member 1 has been set correctly");
        }
        if(vertex->getLabel() ==  "v1")
        {
            DerivedVertexB* v = dynamic_cast<DerivedVertexB*>(vertex.get());
            BOOST_REQUIRE_MESSAGE(v->mMember2 == v1->mMember2,
                    "Vertex 1 member 2 has been set correctly "
                    << v->mMember0 << " vs. " << v1->mMember2
                    );

            BOOST_REQUIRE_MESSAGE(v->mMember0 == v1->mMember0,
                    "Vertex 1 inherited member 0 has been set correctly "
                    << v->mMember0 << " vs. " << v1->mMember0
                    );

            BOOST_REQUIRE_MESSAGE(v->mMember1 == v1->mMember1,
                    "Vertex 1 inherited member 1 has been set correctly");
        }
    }


    EdgeIterator::Ptr edgeIt = read_graph->getEdgeIterator();
    while(edgeIt->next())
    {
        Edge::Ptr edge = edgeIt->current();
        if(edge->getLabel() == "e_A")
        {
            DerivedEdgeA* e = dynamic_cast<DerivedEdgeA*>(edge.get());

            BOOST_REQUIRE_MESSAGE(e->mMember0 == edgeA->mMember0,
                    "Edge A member 0 has been set correctly");
            BOOST_REQUIRE_MESSAGE(e->mMember1 == edgeA->mMember1,
                    "Edge A member 1 has been set correctly");
        }

        if(edge->getLabel() == "e_B")
        {
            DerivedEdgeB* e = dynamic_cast<DerivedEdgeB*>(edge.get());

            BOOST_REQUIRE_MESSAGE(e->mMember2 == edgeB->mMember2,
                    "Edge B member 2 has been set correctly");


            BOOST_REQUIRE_MESSAGE(e->mMember0 == edgeB->mMember0,
                    "Edge B inherited member 0 has been set correctly");

            BOOST_REQUIRE_MESSAGE(e->mMember1 == edgeB->mMember1,
                    "Edge B inherited member 1 has been set correctly");
        }

    }
}

BOOST_AUTO_TEST_CASE(gexf_viz)
{
    BaseGraph::Ptr read_graph = BaseGraph::getInstance();
    std::string filename = getRootDir() + "test/data/yeast.gexf";
    io::GraphIO::read(filename, read_graph, representation::GEXF);
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

BOOST_AUTO_TEST_CASE(graphviz_grid_layout)
{
    BaseGraph::Ptr graph = BaseGraph::getInstance();

    Vertex::Ptr v00(new RowColumnVertex("v0-0",0,0));
    Vertex::Ptr v10(new RowColumnVertex("v1-0",1,0));
    Vertex::Ptr v20(new RowColumnVertex("v2-0",2,0));
    Vertex::Ptr v01(new RowColumnVertex("v0-1",0,1));
    Vertex::Ptr v11(new RowColumnVertex("v1-1",1,1));
    Vertex::Ptr v21(new RowColumnVertex("v2-1",2,1));
    Vertex::Ptr v33(new RowColumnVertex("v3-3",3,3));

    Edge::Ptr e0(new Edge(v00,v10,"e0"));
    Edge::Ptr e1(new Edge(v10,v20,"e1"));

    Edge::Ptr e2(new Edge(v01,v11,"e01->11"));
    Edge::Ptr e3(new Edge(v11,v21,"e11->21"));

    graph->addVertex(v33);
    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);
    graph->addEdge(e3);

    io::GraphvizWriter gvWriter("neato","canon");
    io::GraphvizGridStyle::Ptr style(new io::GraphvizGridStyle(3,3,
                &RowColumnVertex::getRow,
                &RowColumnVertex::getColumn
                ));
    style->setColumnScalingFactor(2.0);
    style->setRowScalingFactor(2.0);
    gvWriter.setStyle(style);

    gvWriter.write("/tmp/graph_analysis-test-graph_io-graphviz_grid_layout.dot", graph);
}

BOOST_AUTO_TEST_CASE(nweight_serialization)
{
    WeightedEdge::Ptr edge(new WeightedEdge(10));
    std::string s = edge->serializeWeights();

    BOOST_TEST_MESSAGE("Serialized weight is: '" << s << "'");

    WeightedEdge::Ptr deserializedEdge(new WeightedEdge());
    deserializedEdge->deserializeWeights(s);

    BOOST_REQUIRE_MESSAGE(edge->getWeight() == deserializedEdge->getWeight(), "Expected weight " << edge->getWeight() << " got " << deserializedEdge->getWeight());

    BaseGraph::Ptr graph = BaseGraph::getInstance();
    Vertex::Ptr v0(new Vertex("v0"));
    Vertex::Ptr v1(new Vertex("v1"));

    edge->setSourceVertex(v0);
    edge->setTargetVertex(v0);
    graph->addEdge(edge);

    EdgeTypeManager* eManager = EdgeTypeManager::getInstance();
    eManager->registerType(edge);
    eManager->registerAttribute(edge->getClassName(), "weights",
            (io::AttributeSerializationCallbacks::serialize_func_t)&WeightedEdge::serializeWeights,
            (io::AttributeSerializationCallbacks::deserialize_func_t)&WeightedEdge::deserializeWeights,
            (io::AttributeSerializationCallbacks::print_func_t)&WeightedEdge::serializeWeights);


    io::GraphIO::write("/tmp/graph_analysis-test-graph_io-nweight-serialization.gexf", graph, representation::GEXF);
}

BOOST_AUTO_TEST_CASE(graphml)
{
    WeightedEdge::Ptr edge(new WeightedEdge(10));
    BOOST_TEST_MESSAGE("NAME: " << edge->getClassName());

    std::string s = edge->serializeWeights();

    BOOST_TEST_MESSAGE("Serialized weight is: '" << s << "'");

    WeightedEdge::Ptr deserializedEdge(new WeightedEdge());
    deserializedEdge->deserializeWeights(s);

    BOOST_REQUIRE_MESSAGE(edge->getWeight() == deserializedEdge->getWeight(), "Expected weight " << edge->getWeight() << " got " << deserializedEdge->getWeight());

    BaseGraph::Ptr graph = BaseGraph::getInstance();
    Vertex::Ptr v0(new Vertex("v0"));
    Vertex::Ptr v1(new Vertex("v1"));

    edge->setSourceVertex(v0);
    edge->setTargetVertex(v1);
    graph->addEdge(edge);

    std::vector<representation::Type> testTypes = { representation::GEXF,
        representation::GRAPHML };

    for(representation::Type type : testTypes)
    {
        BOOST_TEST_MESSAGE("Testing representation: " <<
                representation::TypeTxt[ (representation::Type) type ] );
        std::string filename = "/tmp/graph_analysis-test-graph_io";
        filename = io::GraphIO::write(filename, graph, (representation::Type) type);

        BaseGraph::Ptr readGraph = BaseGraph::getInstance();
        io::GraphIO::read(filename, readGraph);

        EdgeIterator::Ptr edgeIt = readGraph->getEdgeIterator();
        BOOST_REQUIRE_EQUAL(readGraph->size(),1);
        BOOST_REQUIRE_EQUAL(readGraph->order(),2);

        while(edgeIt->next())
        {
            Edge::Ptr currentEdge = edgeIt->current();
            WeightedEdge::Ptr weightedEdge =
                dynamic_pointer_cast<WeightedEdge>(currentEdge);
            BOOST_REQUIRE_MESSAGE(weightedEdge->getWeight(0) == edge->getWeight(0),
                    "Expected weight " << edge->getWeight(0));
        }
    }
}

BOOST_AUTO_TEST_CASE(identify_file_type)
{
    std::vector<std::string> suffixes = { "dot", "gexf", "graphml" };
    for(const std::string& suffix : suffixes)
    {
        representation::Type representationType = io::GraphIO::getTypeFromSuffix(suffix);
        BOOST_REQUIRE_MESSAGE( io::GraphIO::getTypeFromFilename("test." + suffix) == representationType, suffix << " suffixed file correctly identify identified as " << representationType);
    }

    BaseGraph::Ptr baseGraph = BaseGraph::getInstance();
    Vertex::Ptr v = make_shared<Vertex>("test");
    baseGraph->addVertex(v);
    graph_analysis::io::GraphIO::write("/tmp/organization-model-connectivity-test-payload-10.dot", baseGraph);

}

BOOST_AUTO_TEST_SUITE_END()
