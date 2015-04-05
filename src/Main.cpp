#include <iostream>
#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/snap/Graph.hpp>
#include <base/Time.hpp>

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
        std::cout << "    add     (p node): " << costAddNodes() << " s" << std::endl;
        std::cout << "    get     (p node): " << costGetNodes() << " s" << std::endl;
        std::cout << "    add     (p edge): " << costAddEdges() << " s" << std::endl;
        std::cout << "    iterate (p node): " << costIterateNodes() << " s" << std::endl;
        std::cout << "    iterate (p edge): " << costIterateEdges() << " s" << std::endl;

    }
};

int main(int argc, char** argv)
{
    using namespace graph_analysis;

    Benchmark lemonMark("lemon");
    Benchmark lemonRawMark("lemon-raw");
    Benchmark snapMark("snap");
    Benchmark snapRawMark("snap-raw");
    int nodeMax = 100000;
    int edgeMax = 100000;
    {
        graph_analysis::lemon::DirectedGraph graph;
        lemonMark.numberOfNodes = nodeMax;
        lemonMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            Vertex::Ptr v(new Vertex());
            graph.addVertex( v );
        }
        lemonMark.stopAddNodes = base::Time::now();

        lemonMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            graph.getVertex(i);
        }
        lemonMark.stopGetNodes = base::Time::now();

        VertexIterator::Ptr vertexIt = graph.getVertexIterator();
        lemonMark.startIterateNodes = base::Time::now();
        while(vertexIt->next())
        {
            Vertex::Ptr v = vertexIt->current();
        }
        lemonMark.stopIterateNodes = base::Time::now();

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

        EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
        lemonMark.startIterateEdges = base::Time::now();
        while(edgeIt->next())
        {
            Edge::Ptr e = edgeIt->current();
        }
        lemonMark.stopIterateEdges = base::Time::now();
    }
    {
        graph_analysis::lemon::DirectedGraph graph;
        ::lemon::ListDigraph& rawGraph = graph.raw();

        lemonRawMark.numberOfNodes = nodeMax;
        lemonRawMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph.addNode();
        }
        lemonRawMark.stopAddNodes = base::Time::now();

        lemonRawMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph.nodeFromId(i);
        }
        lemonRawMark.stopGetNodes = base::Time::now();

        lemonRawMark.startIterateNodes = base::Time::now();
        for (::lemon::ListDigraph::NodeIt n(rawGraph); n != ::lemon::INVALID; ++n)
        {
            rawGraph.id(n);
        }
        lemonRawMark.stopIterateNodes = base::Time::now();

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

        lemonRawMark.startIterateEdges = base::Time::now();
        for(::lemon::ListDigraph::ArcIt a(rawGraph); a != ::lemon::INVALID; ++a)
        {
            Edge::Ptr edge = edgeMap[a];
        }
        lemonRawMark.stopIterateEdges = base::Time::now();
    }

    {
        graph_analysis::snap::DirectedGraph graph;
        snapMark.numberOfNodes = nodeMax;
        snapMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            Vertex::Ptr v(new Vertex());
            graph.addVertex( v );
        }
        snapMark.stopAddNodes = base::Time::now();

        snapMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            graph.getVertex(i);
        }
        snapMark.stopGetNodes = base::Time::now();

        VertexIterator::Ptr vertexIt = graph.getVertexIterator();
        snapMark.startIterateNodes = base::Time::now();
        while(vertexIt->next())
        {
            Vertex::Ptr v = vertexIt->current();
        }
        snapMark.stopIterateNodes = base::Time::now();

        // Edges
        snapMark.numberOfEdges = edgeMax;
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

        EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
        snapMark.startIterateEdges = base::Time::now();
        while(edgeIt->next())
        {
            Edge::Ptr e = edgeIt->current();
        }
        snapMark.stopIterateEdges = base::Time::now();
    }

    {
        graph_analysis::snap::DirectedGraph graph;
        graph_analysis::snap::DirectedGraph::graph_t& rawGraph  = graph.raw();
        snapRawMark.numberOfNodes = nodeMax;
        snapRawMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph.AddNode();
        }
        snapRawMark.stopAddNodes = base::Time::now();

        snapRawMark.startGetNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph.GetNI(i);
        }
        snapRawMark.stopGetNodes = base::Time::now();

        snapRawMark.startIterateNodes = base::Time::now();
        graph_analysis::snap::DirectedGraph::graph_t::TNodeI nodeIt = rawGraph.BegNI();
        for (; nodeIt < rawGraph.EndNI(); nodeIt++)
        {
            rawGraph.GetNDat(nodeIt.GetId()).value;
        }
        snapRawMark.stopIterateNodes = base::Time::now();

        // Edges
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
