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

    base::Time startAddEdges;
    base::Time stopAddEdges;

    double costAddNodes() { return (stopAddNodes - startAddNodes).toSeconds() / numberOfNodes; }

    double costAddEdges() { return (stopAddEdges - startAddEdges).toSeconds() / numberOfEdges; }

    void printReport()
    {
        std::cout << "Benchmark: " << label << std::endl;
        std::cout << "    add (p node): " << costAddNodes() << " s" << std::endl;
        std::cout << "    add (p edge): " << costAddEdges() << " s" << std::endl;
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
    }

    {
        graph_analysis::snap::DirectedGraph graph;
        PNEGraph& rawGraph  = graph.raw();
        snapRawMark.numberOfNodes = nodeMax;
        snapRawMark.startAddNodes = base::Time::now();
        for(int i = 0; i < nodeMax; ++i)
        {
            rawGraph->AddNode();
        }
        snapRawMark.stopAddNodes = base::Time::now();
    }

    lemonMark.printReport();
    lemonRawMark.printReport();
    snapMark.printReport();
    snapRawMark.printReport();

    return 0;
}
