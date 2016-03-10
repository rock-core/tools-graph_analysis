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
 *    - support of parallel arcs in digraphs, see also: http://lemon.cs.elte.hu/pub/tutorial/a00011.html#sec_digraphs
 *
 * After short evaluation lemon has been selected as first choice, since it seems to provide a good combination of a properly implemented build,
 * usability, functionality and performance compared to other libraries.
 *
 * Further resources:
 *  - http://stackoverflow.com/questions/2751826/which-c-graph-library-should-i-use
 *  - http://stackoverflow.com/questions/3010805/scalable-parallel-large-graph-analysis-library
 *
 * \section Motivation
 * A number of different graph libraries does exist - as listed above - though they cannot always be compared in a one to one fashion.
 * This library tries to provide access to (at first one) and possibly further graph libraries to give access
 * to a larger set of functionality.
 * In the course of development we might see that including additional libraries leads to an overcomplicated
 * wrapping infrastructure.
 * The goal however it to maintain a consistent and capable high-level interface abstraction in order to allow graph modeling and analysis.
*
 * \section Examples
 \verbatim
 #include <graph_analysis/GraphAnalysis.hpp>
 #include <graph_analysis/lemon/Graph.hpp>
 ...
 using namespace graph_analysis;

 class MyVertex : Vertex
 {
 public:
     MyVertex(const std::string& attr)
         : mAttribute(attr)
     {}

     std::string getClassName() const { return "MyVertex"; }
     std::string toString() const { return getClassName() + " " + mAttribute; }
 protected:
     /// To allow for cloning of graphs using deep copy
     Vertex* getClone() const { return new MyVertex(*this); }
 private:
     std::string mAttribute;
 };

 BaseGraph::Ptr graph = BaseGraph::getInstance(BaseGraph::LEMON_DIRECTED_GRAPH);
 Vertex::Ptr v0(new MyVertex());
 Vertex::Ptr v1(new MyVertex());

 Edge::Ptr e0(new Edge());
 e0->setSourceVertex(v0);
 e0->setTargetVertex(v1);

 graph->add(e0);

 int vertexCount = graph->getVertexCount();
 int vertexCountInternal = ::lemon::countNodes(graph.raw());

 VertexIterator::Ptr vertexIt = graph->getVertexIterator();
 while(vertexIt->next())
 {
    Vertex::Ptr vertex = vertexIt->current();
    printf("Vertex: %s\n", vertex->toString().c_str());
 }

 EdgeIterator::Ptr edgeIt = graph->getEdgeIterator();
 while(edgeIt->next())
 {
    Edge::Ptr edge = edgeIt->current();
    printf("Edge: %s\n", edge->toString().c_str());
 }

 \endverbatim
 *
 * \section Graph I/O
 * \see Detailed description of graph_analysis::io::GraphIO
 */

/**
 * \namespace graph_analysis
 * \brief Main namespace of the graph analysis library
 *
 * \namespace algorithms
 * \brief Algorithms implementations that rely on the (wrapper) interface of this library
 */
#include <graph_analysis/Graph.hpp>

#endif // GRAPH_ANALYSIS_GRAPH_ANALYSIS_HPP
