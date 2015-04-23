#include <iostream>
#include <boost/lexical_cast.hpp>
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

    double costAddNodes() { return (stopAddNodes - startAddNodes).toSeconds() / numberOfNodes; }
    double costGetNodes() { return (stopGetNodes - startGetNodes).toSeconds() / numberOfNodes; }
    double costAddEdges() { return (stopAddEdges - startAddEdges).toSeconds() / numberOfEdges; }
    double costIterateNodes() { return (stopIterateNodes - startIterateNodes).toSeconds() / numberOfNodes; }
    double costIterateEdges() { return (stopIterateEdges - startIterateEdges).toSeconds() / numberOfEdges; }

    void printReport()
    {
        std::cout << "Benchmark: " << label << std::endl;
        std::cout << "    number of nodes:  " << numberOfNodes << std::endl;
        std::cout << "    number of edges:  " << numberOfEdges << std::endl;
        std::cout << "    add     (p node): " << costAddNodes() << " s" << std::endl;
        std::cout << "    get     (p node): " << costGetNodes() << " s" << std::endl;
        std::cout << "    add     (p edge): " << costAddEdges() << " s" << std::endl;
        std::cout << "    iterate (p node): " << costIterateNodes() << " s" << std::endl;
        std::cout << "    iterate (p edge): " << costIterateEdges() << " s" << std::endl;

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

    using namespace graph_analysis;

    Benchmark lemonMark("lemon");
    Benchmark lemonRawMark("lemon-raw");
    Benchmark snapMark("snap");
    Benchmark snapRawMark("snap-raw");
    {
        printf("lemon -- with wrapper\n");
        graph_analysis::lemon::DirectedGraph graph;
        lemonMark.numberOfNodes = nodeMax;
        printf("    -- adding nodes\n");
        lemonMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            Vertex::Ptr v(new Vertex());
            graph.addVertex( v );
        }
        lemonMark.stopAddNodes = base::Time::now();

        printf("    -- getting nodes\n");
        lemonMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            graph.getVertex(i);
        }
        lemonMark.stopGetNodes = base::Time::now();

        printf("    -- iterating nodes\n");
        VertexIterator::Ptr vertexIt = graph.getVertexIterator();
        lemonMark.startIterateNodes = base::Time::now();
        while(vertexIt->next())
        {
            Vertex::Ptr v = vertexIt->current();
        }
        lemonMark.stopIterateNodes = base::Time::now();

        printf("    -- add edges -- with existing nodes\n");
        // Edges
        lemonMark.numberOfEdges = edgeMax;
        lemonMark.startAddEdges = base::Time::now();
        for(int i = 0; i < edgeMax; ++i)
        {
            Vertex::Ptr v0(new Vertex());
            Vertex::Ptr v1(new Vertex());
            Edge::Ptr e0(new Edge());
            e0->setSourceVertex(v0);
            e0->setTargetVertex(v1);
            graph.addEdge(e0);
        }
        lemonMark.stopAddEdges = base::Time::now();

        printf("    -- iterate edges\n");
        EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
        lemonMark.startIterateEdges = base::Time::now();
        while(edgeIt->next())
        {
            Edge::Ptr e = edgeIt->current();
        }
        lemonMark.stopIterateEdges = base::Time::now();
    }
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
            edgeMap[arc] = Edge::Ptr(new Edge());
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

    {
        printf("snap -- with wrapper\n");
        graph_analysis::snap::DirectedGraph graph;
        snapMark.numberOfNodes = nodeMax;

        printf("    -- adding nodes\n");
        snapMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            Vertex::Ptr v(new Vertex());
            graph.addVertex( v );
        }
        snapMark.stopAddNodes = base::Time::now();

        printf("    -- getting nodes\n");
        snapMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            graph.getVertex(i);
        }
        snapMark.stopGetNodes = base::Time::now();

        printf("    -- iterating nodes\n");
        VertexIterator::Ptr vertexIt = graph.getVertexIterator();
        snapMark.startIterateNodes = base::Time::now();
        while(vertexIt->next())
        {
            Vertex::Ptr v = vertexIt->current();
        }
        snapMark.stopIterateNodes = base::Time::now();

        // Edges
        snapMark.numberOfEdges = edgeMax;
        printf("    -- add edges -- with existing nodes\n");
        snapMark.startAddEdges = base::Time::now();
        for(int i = 0; i < edgeMax; ++i)
        {
            Vertex::Ptr v0(new Vertex());
            Vertex::Ptr v1(new Vertex());
            Edge::Ptr e0(new Edge());
            e0->setSourceVertex(v0);
            e0->setTargetVertex(v1);
            graph.addEdge(e0);
        }
        snapMark.stopAddEdges = base::Time::now();

        printf("    -- iterate edges\n");
        EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
        snapMark.startIterateEdges = base::Time::now();
        while(edgeIt->next())
        {
            Edge::Ptr e = edgeIt->current();
        }
        snapMark.stopIterateEdges = base::Time::now();
    }

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

            Vertex::Ptr v0(new Vertex());
            Vertex::Ptr v1(new Vertex());
            snap::Serializable<Vertex::Ptr> s0(v0);
            snap::Serializable<Vertex::Ptr> s1(v1);

            Edge::Ptr e0(new Edge());
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

    lemonMark.printReport();
    lemonRawMark.printReport();
    snapMark.printReport();
    snapRawMark.printReport();

    return 0;
}
