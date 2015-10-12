#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <base/Time.hpp>

#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/snap/Graph.hpp>

struct Benchmark
{
    Benchmark(const std::string& _label)
        : label(_label)
    {}

    std::string label;

    int numberOfNodes;
    int numberOfEdges;

    base::Time startAddNodes;
    base::Time stopAddNodes;

    base::Time startGetNodes;
    base::Time stopGetNodes;

    base::Time startAddEdges;
    base::Time stopAddEdges;

    base::Time startIterateNodes;
    base::Time stopIterateNodes;

    base::Time startIterateEdges;
    base::Time stopIterateEdges;

    base::Time startStlIterateNodes;
    base::Time stopStlIterateNodes;

    base::Time startStlIterateEdges;
    base::Time stopStlIterateEdges;


    double costAddNodes() const { return (stopAddNodes - startAddNodes).toSeconds() / numberOfNodes; }
    double costGetNodes() const { return (stopGetNodes - startGetNodes).toSeconds() / numberOfNodes; }
    double costAddEdges() const { return (stopAddEdges - startAddEdges).toSeconds() / numberOfEdges; }
    double costIterateNodes() const { return (stopIterateNodes - startIterateNodes).toSeconds() / numberOfNodes; }
    double costIterateEdges() const { return (stopIterateEdges - startIterateEdges).toSeconds() / numberOfEdges; }
    double costIterateStlEdges() const { return (stopStlIterateEdges - startStlIterateEdges).toSeconds() / numberOfEdges; }
    double costIterateStlNodes() const { return (stopStlIterateNodes - startStlIterateNodes).toSeconds() / numberOfNodes; }

    void printReport() const
    {
        std::cout << "Benchmark: " << label << std::endl;
        std::cout << "    number of nodes:  " << numberOfNodes << std::endl;
        std::cout << "    number of edges:  " << numberOfEdges << std::endl;
        std::cout << "    add     (p node): " << costAddNodes() << " s" << std::endl;
        std::cout << "    get     (p node): " << costGetNodes() << " s" << std::endl;
        std::cout << "    add     (p edge): " << costAddEdges() << " s" << std::endl;
        std::cout << "    iterate (p node): " << costIterateNodes() << " s" << std::endl;
        std::cout << "    iterate (p edge): " << costIterateEdges() << " s" << std::endl;
        std::cout << "stl iterate (p node): " << costIterateStlNodes() << " s" << std::endl;
        std::cout << "stl iterate (p edge): " << costIterateStlEdges() << " s" << std::endl;

    }
};

int main(int argc, char** argv)
{
    int nodeMax = 10000000;
    int edgeMax = 10000000;

    if(argc < 3)
    {
        printf("usage: %s [-h|--help] <number-of-nodes> <number-of-edges>\n", argv[0]);
        exit(0);
    }
    nodeMax = boost::lexical_cast<int>(argv[1]);
    edgeMax = boost::lexical_cast<int>(argv[2]);

    std::vector<Benchmark> benchmarks;

    using namespace graph_analysis;
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        std::string graphName = graph->getImplementationTypeName();
        Benchmark graphMark(graphName);

        std::cout << graphName << " -- with wrapper" << std::endl;

        graphMark.numberOfNodes = nodeMax;
        std::cout << "    -- adding nodes" << std::endl;
        graphMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            Vertex::Ptr v = boost::make_shared<Vertex>();
            graph->addVertex( v );
        }
        graphMark.stopAddNodes = base::Time::now();

        std::cout << "    -- getting nodes" << std::endl;
        graphMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            graph->getVertex(i);
        }
        graphMark.stopGetNodes = base::Time::now();

        std::cout << "    -- iterating nodes" << std::endl;
        VertexIterator::Ptr vertexIt = graph->getVertexIterator();
        graphMark.startIterateNodes = base::Time::now();
        while(vertexIt->next())
        {
            Vertex::Ptr v = vertexIt->current();
        }
        graphMark.stopIterateNodes = base::Time::now();

        std::cout << "    -- add edges -- with existing nodes" << std::endl;
        // Edges
        graphMark.numberOfEdges = edgeMax;
        graphMark.startAddEdges = base::Time::now();
        for(int i = 0; i < edgeMax; ++i)
        {
            Vertex::Ptr v0 = boost::make_shared<Vertex>();
            Vertex::Ptr v1 = boost::make_shared<Vertex>();
            Edge::Ptr e0 = boost::make_shared<Edge>();
            e0->setSourceVertex(v0);
            e0->setTargetVertex(v1);
            graph->addEdge(e0);
        }
        graphMark.stopAddEdges = base::Time::now();

        std::cout << "    -- iterate edges" << std::endl;
        EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
        graphMark.startIterateEdges = base::Time::now();
        while(edgeIt->next())
        {
            Edge::Ptr e = edgeIt->current();
        }
        graphMark.stopIterateEdges = base::Time::now();

        std::cout << "    -- iterate (stl) nodes" << std::endl;
        graphMark.startStlIterateNodes = base::Time::now();

        typedef BaseIterable<VertexIterator::Ptr, BaseGraph, Vertex::Ptr> IterableVertices;
        IterableVertices::Iterator vit = graph->vertices().begin();
        for(; vit != graph->vertices().end(); ++vit)
        {
            Vertex::Ptr vertex = *vit;
        }
        graphMark.stopStlIterateNodes = base::Time::now();

        std::cout << "    -- iterate (stl) edges" << std::endl;
        graphMark.startStlIterateEdges = base::Time::now();

        typedef BaseIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr> IterableEdges;
        IterableEdges::Iterator eit = graph->edges().begin();
        for(; eit != graph->edges().end(); ++eit)
        {
            Edge::Ptr edge = *eit;
        }
        graphMark.stopStlIterateEdges = base::Time::now();

        benchmarks.push_back(graphMark);
    }

    Benchmark lemonRawMark("lemon-raw");
    {
        printf("lemon -- raw\n");
        graph_analysis::lemon::DirectedGraph graph;
        ::lemon::ListDigraph& rawGraph = graph.raw();


        printf("    -- adding nodes\n");
        lemonRawMark.numberOfNodes = nodeMax;
        lemonRawMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph.addNode();
        }
        lemonRawMark.stopAddNodes = base::Time::now();

        printf("    -- getting nodes\n");
        lemonRawMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph.nodeFromId(i);
        }
        lemonRawMark.stopGetNodes = base::Time::now();

        printf("    -- iterating nodes\n");
        lemonRawMark.startIterateNodes = base::Time::now();
        for (::lemon::ListDigraph::NodeIt n(rawGraph); n != ::lemon::INVALID; ++n)
        {
            rawGraph.id(n);
        }
        lemonRawMark.stopIterateNodes = base::Time::now();

        printf("    -- add edges -- with existing nodes\n");
        ::lemon::ListDigraph::ArcMap< Edge::Ptr > edgeMap(rawGraph);
        // Edges
        lemonRawMark.numberOfEdges = edgeMax;
        lemonRawMark.startAddEdges = base::Time::now();
        for(int i = 0; i < edgeMax; ++i)
        {
            ::lemon::ListDigraph::Node sourceNode = rawGraph.addNode();
            ::lemon::ListDigraph::Node targetNode = rawGraph.addNode();
            ::lemon::ListDigraph::Arc arc = rawGraph.addArc(sourceNode,targetNode);
            edgeMap[arc] = boost::make_shared<Edge>();
        }
        lemonRawMark.stopAddEdges = base::Time::now();

        printf("    -- iterate edges\n");
        lemonRawMark.startIterateEdges = base::Time::now();
        for(::lemon::ListDigraph::ArcIt a(rawGraph); a != ::lemon::INVALID; ++a)
        {
            Edge::Ptr edge = edgeMap[a];
        }
        lemonRawMark.stopIterateEdges = base::Time::now();


    }
    benchmarks.push_back(lemonRawMark);

    Benchmark snapRawMark("snap-raw");
    {
        printf("snap -- raw\n");
        graph_analysis::snap::DirectedGraph graph;
        graph_analysis::snap::DirectedGraph::graph_t& rawGraph  = graph.raw();
        snapRawMark.numberOfNodes = nodeMax;
        printf("    -- adding nodes\n");
        snapRawMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph.AddNode();
        }
        snapRawMark.stopAddNodes = base::Time::now();

        printf("    -- getting nodes\n");
        snapRawMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph.GetNI(i);
        }
        snapRawMark.stopGetNodes = base::Time::now();

        printf("    -- iterating nodes\n");
        snapRawMark.startIterateNodes = base::Time::now();
        graph_analysis::snap::DirectedGraph::graph_t::TNodeI nodeIt = rawGraph.BegNI();
        for (; nodeIt < rawGraph.EndNI(); nodeIt++)
        {
            rawGraph.GetNDat(nodeIt.GetId()).value;
        }
        snapRawMark.stopIterateNodes = base::Time::now();

        // Edges
        printf("    -- add edges -- with existing nodes\n");
        snapRawMark.numberOfEdges = edgeMax;
        snapRawMark.startAddEdges = base::Time::now();

        int nodeId = nodeMax;
        for(int i = 0; i < edgeMax; ++i)
        {
            using namespace graph_analysis;

            Vertex::Ptr v0 = boost::make_shared<Vertex>();
            Vertex::Ptr v1 = boost::make_shared<Vertex>();
            snap::Serializable<Vertex::Ptr> s0(v0);
            snap::Serializable<Vertex::Ptr> s1(v1);

            Edge::Ptr e0 = boost::make_shared<Edge>();
            snap::Serializable<Edge::Ptr> se0(e0);
            int sourceNode = rawGraph.AddNode(nodeId++,s0);
            int targetNode = rawGraph.AddNode(nodeId++,s1);

            int edge = rawGraph.AddEdge(sourceNode, targetNode, i, se0);
        }
        snapRawMark.stopAddEdges = base::Time::now();

        printf("    -- iterate edges\n");
        snapRawMark.startIterateEdges = base::Time::now();
        graph_analysis::snap::DirectedGraph::graph_t::TEdgeI edgeIt = rawGraph.BegEI();
        for (; edgeIt < rawGraph.EndEI(); edgeIt++)
        {
            rawGraph.GetEDat(edgeIt.GetId()).value;
        }
        snapRawMark.stopIterateEdges = base::Time::now();
    }
    benchmarks.push_back(snapRawMark);

    std::vector<Benchmark>::const_iterator cit = benchmarks.begin();
    for(; cit != benchmarks.end(); ++cit)
    {
        cit->printReport();
    }
    return 0;
}
