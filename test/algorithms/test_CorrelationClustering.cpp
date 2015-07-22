#include <boost/test/unit_test.hpp>

#include <graph_analysis/lemon/Graph.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/algorithms/CorrelationClustering.hpp>
#include <graph_analysis/WeightedEdge.hpp>

using namespace graph_analysis::algorithms;
using namespace graph_analysis;

BOOST_AUTO_TEST_SUITE(algorithms_correlation_clustering)

double getWeight(Edge::Ptr edge)
{
    WeightedEdge::Ptr weightedEdge = boost::dynamic_pointer_cast<WeightedEdge>(edge);
    return weightedEdge->getWeight();
}

// complete graph with 3 vertices
//
// input:
// v0 - 1.0 - v1
// |          /
// -2.0    1.0
// |     /
// v2
//
// output:
// v0 --- v1
//
//    v3
BOOST_AUTO_TEST_CASE(cluster_correlation_1)
{
    graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1( new Vertex("1"));
    Vertex::Ptr v2( new Vertex("2"));

    graph->addVertex(v0);
    graph->addVertex(v1);
    graph->addVertex(v2);

    WeightedEdge::Ptr e0(new WeightedEdge(1.0));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    WeightedEdge::Ptr e1(new WeightedEdge(1.0));
    e1->setSourceVertex(v1);
    e1->setTargetVertex(v2);

    WeightedEdge::Ptr e2(new WeightedEdge(-2.0));
    e2->setSourceVertex(v2);
    e2->setTargetVertex(v0);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);

    CorrelationClustering cc(graph, getWeight);
    {
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }

    {
        cc.round();
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation after rounding (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }
}


// complete graph with 4 vertices
//
// input:
// v0 -- 10.0  -- v1
// |               |
//       /1.0/
// -3.0           2.0
//       \1.0\
// |               |
// v2 -- 1.0 --   v3
//
// output:
//
// v0 ---- v1
//         |
// v2      v3
BOOST_AUTO_TEST_CASE(cluster_correlation_2)
{
    graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1( new Vertex("1"));
    Vertex::Ptr v2( new Vertex("2"));
    Vertex::Ptr v3( new Vertex("3"));

    graph->addVertex(v0);
    graph->addVertex(v1);
    graph->addVertex(v2);
    graph->addVertex(v3);

    WeightedEdge::Ptr e0(new WeightedEdge(10.0));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    WeightedEdge::Ptr e1(new WeightedEdge(1.0));
    e1->setSourceVertex(v1);
    e1->setTargetVertex(v2);

    WeightedEdge::Ptr e2(new WeightedEdge(1.0));
    e2->setSourceVertex(v2);
    e2->setTargetVertex(v3);

    WeightedEdge::Ptr e3(new WeightedEdge(1.0));
    e3->setSourceVertex(v3);
    e3->setTargetVertex(v0);

    WeightedEdge::Ptr e4(new WeightedEdge(-3.0));
    e4->setSourceVertex(v0);
    e4->setTargetVertex(v2);

    WeightedEdge::Ptr e5(new WeightedEdge(2.0));
    e5->setSourceVertex(v3);
    e5->setTargetVertex(v1);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);
    graph->addEdge(e3);
    graph->addEdge(e4);
    graph->addEdge(e5);


    CorrelationClustering cc(graph, getWeight);
    {
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }

    {
        cc.round();
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation after rounding (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }
}

// complete graph with 5 vertices
//
// input:
//                v1
//       /5.0/
//                \-4.0\
//
//  v0         = -5.0 =        v2
//
//    \-7.0\ /-3.0/
//                /1.0/    /5.0/
//
//              \-6.0\
//         v3  == 3.0 ==  v4
//
//
// output: 2 clusters
//      ___ v1
//     /
// v0       /----  __ v2
//         /        /
//       v3---- v4
//
BOOST_AUTO_TEST_CASE(cluster_correlation_3)
{
    graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1( new Vertex("1"));
    Vertex::Ptr v2( new Vertex("2"));
    Vertex::Ptr v3( new Vertex("3"));
    Vertex::Ptr v4( new Vertex("4"));

    graph->addVertex(v0);
    graph->addVertex(v1);
    graph->addVertex(v2);
    graph->addVertex(v3);
    graph->addVertex(v4);

    WeightedEdge::Ptr e0(new WeightedEdge(5.0));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    WeightedEdge::Ptr e1(new WeightedEdge(-5.0));
    e1->setSourceVertex(v0);
    e1->setTargetVertex(v2);

    WeightedEdge::Ptr e2(new WeightedEdge(-7.0));
    e2->setSourceVertex(v0);
    e2->setTargetVertex(v3);

    WeightedEdge::Ptr e3(new WeightedEdge(-6.0));
    e3->setSourceVertex(v0);
    e3->setTargetVertex(v4);

    WeightedEdge::Ptr e4(new WeightedEdge(-3.0));
    e4->setSourceVertex(v1);
    e4->setTargetVertex(v2);

    WeightedEdge::Ptr e5(new WeightedEdge(-3.0));
    e5->setSourceVertex(v1);
    e5->setTargetVertex(v3);

    WeightedEdge::Ptr e6(new WeightedEdge(-4.0));
    e6->setSourceVertex(v1);
    e6->setTargetVertex(v4);

    WeightedEdge::Ptr e7(new WeightedEdge(1.0));
    e7->setSourceVertex(v2);
    e7->setTargetVertex(v3);

    WeightedEdge::Ptr e8(new WeightedEdge(5.0));
    e8->setSourceVertex(v2);
    e8->setTargetVertex(v4);

    WeightedEdge::Ptr e9(new WeightedEdge(3.0));
    e9->setSourceVertex(v3);
    e9->setTargetVertex(v4);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);
    graph->addEdge(e3);
    graph->addEdge(e4);
    graph->addEdge(e5);
    graph->addEdge(e6);
    graph->addEdge(e7);
    graph->addEdge(e8);
    graph->addEdge(e9);

    CorrelationClustering cc(graph, getWeight);
    {
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }

    {
        cc.round();
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation after rounding (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }
}

/*

          -1
    v0- - - - - v1
    |           |
 +1 |           | +1
    |           |
    v3 -------- v2
          +1

 for e0 = -1 and e1 = e2 = e3 = +1, the algorithm doesn't work
 it returns e0 = 1 and e1 = e2 = e3 = 0 which is not possible
 (if v0, v1, v2, v3 are in the same cluster , than e0 can not be 1)

 Need to add a condition to check if we have 4 vertices, with an edge between each two consecutive
 vertices; then we can not have 3 edges 0 and the other one 1
*/

BOOST_AUTO_TEST_CASE(cluster_correlation_4)
{
    graph_analysis::BaseGraph::Ptr graph(new graph_analysis::lemon::DirectedGraph());

    Vertex::Ptr v0( new Vertex("0"));
    Vertex::Ptr v1( new Vertex("1"));
    Vertex::Ptr v2( new Vertex("2"));
    Vertex::Ptr v3( new Vertex("3"));

    graph->addVertex(v0);
    graph->addVertex(v1);
    graph->addVertex(v2);
    graph->addVertex(v3);

    WeightedEdge::Ptr e0(new WeightedEdge(-1.0));
    e0->setSourceVertex(v0);
    e0->setTargetVertex(v1);

    WeightedEdge::Ptr e1(new WeightedEdge(1.0));
    e1->setSourceVertex(v1);
    e1->setTargetVertex(v2);

    WeightedEdge::Ptr e2(new WeightedEdge(1.0));
    e2->setSourceVertex(v2);
    e2->setTargetVertex(v3);

    WeightedEdge::Ptr e3(new WeightedEdge(1.0));
    e3->setSourceVertex(v3);
    e3->setTargetVertex(v0);

    graph->addEdge(e0);
    graph->addEdge(e1);
    graph->addEdge(e2);
    graph->addEdge(e3);

    CorrelationClustering cc(graph, getWeight);
    {
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }

    {
        cc.round();
        std::map<Edge::Ptr, double> solution = cc.getEdgeActivation();
        BOOST_TEST_MESSAGE("EdgeActivation after rounding (0 means active, 1 means inactive):\n" << CorrelationClustering::toString(solution));
    }
}

BOOST_AUTO_TEST_SUITE_END()
