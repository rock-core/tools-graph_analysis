#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/filesystem.hpp>
#include <base/Time.hpp>
#include <numeric/Stats.hpp>

#include "lemon/Graph.hpp"
#include "snap/Graph.hpp"

struct Benchmark
{
    Benchmark(const std::string& _label)
        : label(_label)
    {}

    std::string label;

    int numberOfNodes;
    int numberOfEdges;

    numeric::Stats<double> addNodesStats;
    numeric::Stats<double> getNodesStats;
    numeric::Stats<double> iterateNodesStats;
    numeric::Stats<double> iterateStlNodesStats;

    numeric::Stats<double> addEdgesStats;
    numeric::Stats<double> getEdgesStats;
    numeric::Stats<double> iterateEdgesStats;
    numeric::Stats<double> iterateStlEdgesStats;

    const numeric::Stats<double>& getAddNodesStats() const { return addNodesStats; }
    const numeric::Stats<double>& getGetNodesStats() const { return getNodesStats; }
    const numeric::Stats<double>& getIterateNodesStats() const { return iterateNodesStats; }
    const numeric::Stats<double>& getIterateStlNodesStats() const { return iterateNodesStats; }

    const numeric::Stats<double>& getAddEdgesStats() const { return addEdgesStats; }
    const numeric::Stats<double>& getGetEdgesStats() const { return getEdgesStats; }
    const numeric::Stats<double>& getIterateEdgesStats() const { return iterateEdgesStats; }
    const numeric::Stats<double>& getIterateStlEdgesStats() const { return iterateStlEdgesStats; }

    std::string getReport() const
    {
        std::stringstream ss;
        ss << "Benchmark: " << label << std::endl;
        ss << "    number of nodes:    " << numberOfNodes << std::endl;
        ss << "    number of edges:    " << numberOfEdges << std::endl;
        ss << "    add     (p node):   " << addNodesStats.mean() << " s +/-" << addNodesStats.stdev() << std::endl;
        ss << "    get     (p node):   " << getNodesStats.mean() << "+/-" << getNodesStats.stdev() << " s" << std::endl;
        ss << "    add     (p edge):   " << addEdgesStats.mean() << "+/-" << addEdgesStats.stdev() <<" s" << std::endl;
        ss << "    get edges (p node): " << getEdgesStats.mean() << "+/-" << getEdgesStats.mean() <<" s" << std::endl;
        ss << "    iterate (p node):   " << iterateNodesStats.mean() << "+/-" << iterateNodesStats.stdev() <<" s" << std::endl;
        ss << "    iterate (p edge):   " << iterateEdgesStats.mean() << "+/-" << iterateEdgesStats.stdev() << " s" << std::endl;
        ss << "stl iterate (p node):   " << iterateStlNodesStats.mean() << "+/-" << iterateStlNodesStats.stdev() <<" s" << std::endl;
        ss << "stl iterate (p edge):   " << iterateStlEdgesStats.mean() << "+/-" << iterateStlEdgesStats.stdev() << " s" << std::endl;
        return ss.str();
    }

    void save(const std::string& logDir)
    {

        std::ios_base::openmode mode = std::ofstream::out | std::ofstream::app;
        std::ofstream file_addNodes    (getLogFilename(logDir,label,"addNodes").c_str(), mode );
        std::ofstream file_getNodes    (getLogFilename(logDir,label,"getNodes").c_str(), mode );
        std::ofstream file_iterateNodes(getLogFilename(logDir,label,"iterateNodes").c_str(), mode);
        std::ofstream file_iterateStlNodes(getLogFilename(logDir,label,"iterateStlNodes").c_str(), mode);

        std::ofstream file_addEdges    (getLogFilename(logDir,label,"addEdges").c_str(), mode);
        std::ofstream file_getEdges    (getLogFilename(logDir,label,"getEdges").c_str(), mode);
        std::ofstream file_iterateEdges(getLogFilename(logDir,label,"iterateEdges").c_str(), mode);
        std::ofstream file_iterateStlEdges(getLogFilename(logDir,label,"iterateStlEdges").c_str(), mode);

        if(addNodesStats.n() > 0)
        {
            file_addNodes << numberOfNodes << " "
                << addNodesStats.mean() << " "
                << addNodesStats.stdev()
                << std::endl;
        }

        if(getNodesStats.n() > 0)
        {
            file_getNodes << numberOfNodes << " "
                << getNodesStats.mean() << " "
                << getNodesStats.stdev()
                << std::endl;
        }

        if(iterateNodesStats.n() > 0)
        {
            file_iterateNodes << numberOfNodes << " "
                << iterateNodesStats.mean() << " "
                << iterateNodesStats.stdev()
                << std::endl;
        }

        if(iterateStlNodesStats.n() > 0)
        {
            file_iterateStlNodes << numberOfNodes << " "
                << iterateStlNodesStats.mean() << " "
                << iterateStlNodesStats.stdev()
                << std::endl;
        }


        if(addEdgesStats.n() > 0)
        {
            file_addEdges << numberOfEdges << " "
                << addEdgesStats.mean() << " "
                << addEdgesStats.stdev()
                << std::endl;
        }

        if(getEdgesStats.n() > 0)
        {
            file_getEdges << numberOfEdges << " "
                << getEdgesStats.mean() << " "
                << getEdgesStats.stdev()
                << std::endl;
        }

        if(iterateEdgesStats.n() > 0)
        {
            file_iterateEdges << numberOfEdges << " "
                << iterateEdgesStats.mean() << " "
                << iterateEdgesStats.stdev()
                << std::endl;
        }

        if(iterateStlEdgesStats.n() > 0)
        {
            file_iterateStlEdges << numberOfEdges << " "
                << iterateStlEdgesStats.mean() << " "
                << iterateStlEdgesStats.stdev()
                << std::endl;
        }
    }

    static std::string getLogFilename(const std::string& dir, const std::string& prefix, const std::string& label)
    {
        return dir + "/" + prefix + "_" + label + ".dat";
    }

};


int main(int argc, char** argv)
{
    int nodeMax = 10000000;
    int edgeMax = 10000000;
    int epochs = 10;
    int splits = 10;

    if(argc < 3 || argc == 4)
    {
        printf("usage: %s [-h|--help] <number-of-nodes> <number-of-edges> <number-of-epochs> <number-of-splits>\n", argv[0]);
        exit(0);
    }
    nodeMax = ::boost::lexical_cast<int>(argv[1]);
    edgeMax = ::boost::lexical_cast<int>(argv[2]);

    if(argc == 5)
    {
        epochs = ::boost::lexical_cast<int>(argv[3]);
        splits = ::boost::lexical_cast<int>(argv[4]);
    }

    std::vector<Benchmark> benchmarks;

    std::string currentTime = base::Time::now().toString(base::Time::Seconds);
    std::string logDir = "/tmp/" + currentTime + "_graph_analysis-benchmark";
    if(! boost::filesystem::create_directories( boost::filesystem::path(logDir.c_str())) )
    {
        throw std::runtime_error("graph_analysis-bm: failed to create log directory: " + logDir);
    }

    base::Time start,stop;

    // Library benchmarking
    using namespace graph_analysis;
    for(int i = BaseGraph::BOOST_DIRECTED_GRAPH; i < BaseGraph::IMPLEMENTATION_TYPE_END; ++i)
    {
        BaseGraph::Ptr graphX = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));
        std::string graphName = graphX->getImplementationTypeName();

        for (int s=1; s <= splits; ++s)
        {
            Benchmark graphMark(graphName);
            graphMark.numberOfNodes = nodeMax* s / splits;
            graphMark.numberOfEdges = edgeMax* s / splits;

            for(int e = 0; e < epochs; ++e)
            {
                BaseGraph::Ptr graph = BaseGraph::getInstance(static_cast<BaseGraph::ImplementationType>(i));

                //std::cout << "    -- adding nodes" << std::endl;
                start = base::Time::now();
                for(int i = 0; i < graphMark.numberOfNodes; ++i)
                {
                    Vertex::Ptr v = make_shared<Vertex>();
                    graph->addVertex( v );
                }
                stop = base::Time::now();
                graphMark.addNodesStats.update((stop-start).toSeconds());

                //std::cout << "    -- getting nodes" << std::endl;
                start = base::Time::now();
                for(int i = 0; i < graphMark.numberOfNodes; ++i)
                {
                    graph->getVertex(i);
                }
                stop = base::Time::now();
                graphMark.getNodesStats.update((stop-start).toSeconds());

                //std::cout << "    -- iterating nodes" << std::endl;
                VertexIterator::Ptr vertexIt = graph->getVertexIterator();
                start = base::Time::now();
                while(vertexIt->next())
                {
                    Vertex::Ptr v = vertexIt->current();
                }
                stop = base::Time::now();
                graphMark.iterateNodesStats.update((stop-start).toSeconds());

                //std::cout << "    -- iterate (stl) nodes" << std::endl;
                start = base::Time::now();

                typedef BaseIterable<VertexIterator::Ptr, BaseGraph, Vertex::Ptr> IterableVertices;
                IterableVertices::Iterator vit = graph->vertices().begin();
                for(; vit != graph->vertices().end(); ++vit)
                {
                    Vertex::Ptr vertex = *vit;
                }
                stop = base::Time::now();
                graphMark.iterateStlNodesStats.update((stop-start).toSeconds());

                // Edges
                //std::cout << "    -- add edges -- with existing nodes" << std::endl;
                start = base::Time::now();
                for(int i = 0; i < graphMark.numberOfEdges; ++i)
                {
                    Vertex::Ptr v0 = make_shared<Vertex>();
                    Vertex::Ptr v1 = make_shared<Vertex>();
                    Edge::Ptr e0 = make_shared<Edge>();
                    e0->setSourceVertex(v0);
                    e0->setTargetVertex(v1);
                    graph->addEdge(e0);
                }
                stop = base::Time::now();

                graphMark.addEdgesStats.update((stop-start).toSeconds());
                //std::cout << "    -- get edges" << std::endl;
                VertexIterator::Ptr vertex0It = graph->getVertexIterator();
                while(vertex0It->next())
                {
                    VertexIterator::Ptr vertex1It = graph->getVertexIterator();
                    while(vertex1It->next())
                    {
                        start = base::Time::now();
                        graph->getEdges(vertex0It->current(), vertex1It->current());
                        stop = base::Time::now();
                        graphMark.getEdgesStats.update((stop-start).toSeconds());
                    }
                }

                //std::cout << "    -- iterate edges" << std::endl;
                EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
                start = base::Time::now();
                while(edgeIt->next())
                {
                    Edge::Ptr e = edgeIt->current();
                }
                stop = base::Time::now();
                graphMark.iterateEdgesStats.update((stop-start).toSeconds());

                //std::cout << "    -- iterate (stl) edges" << std::endl;
                start = base::Time::now();

                typedef BaseIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr> IterableEdges;
                IterableEdges::Iterator eit = graph->edges().begin();
                for(; eit != graph->edges().end(); ++eit)
                {
                    Edge::Ptr edge = *eit;
                }
                stop = base::Time::now();
                graphMark.iterateStlEdgesStats.update((stop-start).toSeconds());

            } // epochs
            graphMark.save(logDir);
            benchmarks.push_back(graphMark);
        } // splits
    }

    // LEMON
    {
        for (int s=1; s <= splits; ++s)
        {
            Benchmark graphMark("lemon");
            graphMark.numberOfNodes = nodeMax * s / splits;
            graphMark.numberOfEdges = edgeMax * s / splits;

            for(int e = 0; e < epochs; ++e)
            {
                graph_analysis::lemon::DirectedGraph graph;
                ::lemon::ListDigraph& rawGraph = graph.raw();

                // Add nodes
                start = base::Time::now();
                for(int i = 0; i < graphMark.numberOfNodes; ++i)
                {
                    rawGraph.addNode();
                }
                stop = base::Time::now();
                graphMark.addNodesStats.update((stop -start).toSeconds());

                // Get nodes
                start = base::Time::now();
                for(int i = 0; i < graphMark.numberOfNodes; ++i)
                {
                    rawGraph.nodeFromId(i);
                }
                stop = base::Time::now();
                graphMark.getNodesStats.update((stop-start).toSeconds());

                //printf("    -- iterating nodes\n");
                start = base::Time::now();
                for (::lemon::ListDigraph::NodeIt n(rawGraph); n != ::lemon::INVALID; ++n)
                {
                    rawGraph.id(n);
                }
                stop = base::Time::now();
                graphMark.iterateNodesStats.update((stop-start).toSeconds());

                //printf("    -- add edges -- with existing nodes\n");
                ::lemon::ListDigraph::ArcMap< Edge::Ptr > edgeMap(rawGraph);
                // Edges
                start = base::Time::now();
                for(int i = 0; i < graphMark.numberOfEdges; ++i)
                {
                    ::lemon::ListDigraph::Node sourceNode = rawGraph.addNode();
                    ::lemon::ListDigraph::Node targetNode = rawGraph.addNode();
                    ::lemon::ListDigraph::Arc arc = rawGraph.addArc(sourceNode,targetNode);
                    edgeMap[arc] = make_shared<Edge>();
                }
                stop = base::Time::now();
                graphMark.addEdgesStats.update((stop-start).toSeconds());

                //printf("    -- iterate edges\n");
                start = base::Time::now();
                for(::lemon::ListDigraph::ArcIt a(rawGraph); a != ::lemon::INVALID; ++a)
                {
                    Edge::Ptr edge = edgeMap[a];
                }
                stop = base::Time::now();
                graphMark.iterateEdgesStats.update((stop-start).toSeconds());
            } // epochs
            graphMark.save(logDir);
            benchmarks.push_back(graphMark);
        } // splits
    }

    {
        for(int s = 1; s <= splits; ++s)
        {
            Benchmark graphMark("snap");
            graphMark.numberOfNodes = nodeMax * s / splits;
            graphMark.numberOfEdges = edgeMax * s / splits;

            for(int e = 0; e < epochs; ++e)
            {
                //printf("snap -- raw\n");
                graph_analysis::snap::DirectedGraph graph;
                graph_analysis::snap::DirectedGraph::graph_t& rawGraph  = graph.raw();

                //printf("    -- adding nodes\n");
                start = base::Time::now();
                for(int i = 0; i < graphMark.numberOfNodes; ++i)
                {
                    rawGraph.AddNode();
                    //printf("%d ",i);
                }
                stop = base::Time::now();
                graphMark.addNodesStats.update((stop-start).toSeconds());

                //printf("    -- getting nodes\n");
                start = base::Time::now();
                for(int i = 0; i < graphMark.numberOfNodes; ++i)
                {
                    rawGraph.GetNI(i);
                }
                stop = base::Time::now();
                graphMark.getNodesStats.update((stop-start).toSeconds());

                //printf("    -- iterating nodes\n");
                start = base::Time::now();
                graph_analysis::snap::DirectedGraph::graph_t::TNodeI nodeIt = rawGraph.BegNI();
                for (; nodeIt < rawGraph.EndNI(); nodeIt++)
                {
                    rawGraph.GetNDat(nodeIt.GetId()).value;
                }
                stop = base::Time::now();
                graphMark.iterateNodesStats.update((stop-start).toSeconds());

                // Edges
                //printf("    -- add edges -- with existing nodes\n");
                start = base::Time::now();
                int nodeId = graphMark.numberOfNodes;
                for(int i = 0; i < graphMark.numberOfEdges; ++i)
                {
                    using namespace graph_analysis;

                    Vertex::Ptr v0 = make_shared<Vertex>();
                    Vertex::Ptr v1 = make_shared<Vertex>();
                    snap::Serializable<Vertex::Ptr> s0(v0);
                    snap::Serializable<Vertex::Ptr> s1(v1);

                    Edge::Ptr e0 = make_shared<Edge>();
                    snap::Serializable<Edge::Ptr> se0(e0);
                    int sourceNode = rawGraph.AddNode(nodeId++,s0);
                    int targetNode = rawGraph.AddNode(nodeId++,s1);
                    rawGraph.AddEdge(sourceNode, targetNode, i, se0);
                }
                stop = base::Time::now();
                graphMark.addEdgesStats.update((stop-start).toSeconds());

                //printf("    -- iterate edges\n");
                start = base::Time::now();
                graph_analysis::snap::DirectedGraph::graph_t::TEdgeI edgeIt = rawGraph.BegEI();
                for (; edgeIt < rawGraph.EndEI(); edgeIt++)
                {
                    rawGraph.GetNDat(edgeIt.GetId()).value;
                }
                stop = base::Time::now();
                graphMark.iterateEdgesStats.update((stop-start).toSeconds());
            } // epochs
            graphMark.save(logDir);
            benchmarks.push_back(graphMark);
        }
    }

    std::ofstream logFile(Benchmark::getLogFilename(logDir,"benchmark","report").c_str());
    std::vector<Benchmark>::const_iterator cit = benchmarks.begin();
    for(; cit != benchmarks.end(); ++cit)
    {
        std::cout << cit->getReport();
        logFile << cit->getReport();
    }
    std::cout << "Logs can be found in " << logDir << std::endl;
    std::cout << "You can use the gnuplot script test/gnuplot_BenchmarkResults.plt to render the results into graphs" << std::endl;

    return 0;
}
