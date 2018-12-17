#ifndef GRAPH_ANALYSIS_GRAPH_ANALYSIS_HPP
#define GRAPH_ANALYSIS_GRAPH_ANALYSIS_HPP

/**
  * \namespace graph_analysis
  * \brief Main namespace of the graph analysis library
  *
  * \namespace graph_analysis::algorithms
  * \brief Algorithms implementations that rely on the (wrapper) interface of this library: breadth-first search,
  * depth-first search, multi-commodity min-cost flow
  *
  * \namespace graph_analysis::filters
  * \brief Collection of Vertex- and Edge-Filters that can be applied to a Graph
  *
  * \namespace graph_analysis::gui
  * \brief A mini Qt-based graphical-framework in order to develop graph-based
  * UIs - including graph-layouting (with graphviz)
  *
  * \namespace graph_analysis::io
  * \brief IO Classes to read/write a graph to standard formats including GEXF
  *
  * \namespace graph_analysis::percolation
  * \brief Classes for simulation percolation processes in a network, e.g.,
  * random removal processes of vertices and edges
  *
  * \namespace graph_analysis::lemon
  * \brief Implementation of the wrapper functionality for the graph library
  * LEMON
  *
  * \namespace graph_analysis::snap
  * \brief Implementation of the wrapper functionality for the graph library
  * SNAP
  *
  * \namespace graph_analysis::boost_graph
  * \brief Implementation of the wrapper functionality for the graph library
  * Boost -- to avoid conflicts of the namespace boost_graph is used as name
  * here
  *
  * \namespace graph_analysis::utils
  * \brief Implementation of utilities such as a md5digest and filesystem
  * helpers
  */
#include "Graph.hpp"

#endif // GRAPH_ANALYSIS_GRAPH_ANALYSIS_HPP
