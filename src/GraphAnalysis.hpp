#ifndef GRAPH_ANALYSIS_GRAPH_ANALYSIS_HPP
#define GRAPH_ANALYSIS_GRAPH_ANALYSIS_HPP

/**
 * \mainpage Graph analysis
 *
 * This library provides functionality for analyzing graphs and networks.
 * Its core functionality will come from other libraries such as:
 *  - igraph: http://igraph.sourceforge.net/
 *    - C API
 *  - boost graph: http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/index.html
 *    - header only
 *    - C++, template-based API
 *  - graph-tool: http://graph-tool.skewed.de/
 *    - boost graph based, with Python integration
 *    - autotools based build, very high compilation requirements
 *  - odgf: http://ogdf.net/doku.php
 *    - layout algorithms
 *  - snap: http://snap.stanford.edu/snap/
 *    - autotools based build
 *  - lemon: http://lemon.cs.elte.hu/trac/lemon
 *    - cmake based build
 *    - support of parallel arces in digraphs, see also: http://lemon.cs.elte.hu/pub/tutorial/a00011.html#sec_digraphs
 *
 * After short evaluation lemon has been selected as first choice, since it seems to provide a good combination of a properly implemented build,
 * usability, functionality and performance compared to other libraries.
 *
 * Further resources:
 *  - http://stackoverflow.com/questions/2751826/which-c-graph-library-should-i-use
 *  - http://stackoverflow.com/questions/3010805/scalable-parallel-large-graph-analysis-library
 *
 * \section Motivation
 * A number of different graph libraries does exist - as listed above - though they cannot always compared in a one to one fashion.
 * This library tries to provide access to (at first one) and possibly further graph libraries to give access
 * to a larger set of functionality.
 * In the course of developement we might see that including additional libraries leads to an overcomplicated
 * wrapping infrastructure.
 * The goal however it to maintain a consistent and capable high-level interface abstraction in order to allow graph modeling and analysis.
*
 * \section Examples
 \verbatim
 #include <graph_analysis/GraphAnalysis.hpp>
 ...
 using namespace graph_analysis;

 class MyVertex : Vertex
 {
     std::string getClassName() const { return "MyVertex"; }
 };

 graph_analysis::lemon::DirectedGraph graph;
 Vertex::Ptr v0(new MyVertex());
 Vertex::Ptr v1(new MyVertex());

 Edge::Ptr e0(new Edge());
 e0->setSourceVertex(v0);
 e0->setTargetVertex(v1);

 int vertexCount = ::lemon::countNodes(graph.raw());

 EdgeIterator::Ptr edgeIt = graph.getEdgeIterator();
 while(edgeIt->next())
 {
    Edge::Ptr edge = edgeIt->current();
    printf("Edge: %s\n", edge->toString().c_str());
 }

 \endverbatim
 */

/**
 * \namespace graph_analysis
 * \brief Main namespace of the graph analyis library
 *
 * \namespace algorithms
 * \brief Algorithms implementations that rely on the (wrapper) interface of this library
 */
#include <graph_analysis/Graph.hpp>

#endif // GRAPH_ANALYSIS_GRAPH_ANALYSIS_HPP
