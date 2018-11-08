# Graph Analysis

This library provides functionality for analyzing graphs and networks.
Its core functionality will come from other graph libraries such as:
 - boost graph: http://www.boost.org/doc/libs/1_55_0/libs/graph/doc/index.html
   - header only
   - C++, template-based API
 - snap: http://snap.stanford.edu/snap/
   - autotools based build
 - lemon: http://lemon.cs.elte.hu/trac/lemon
   - cmake based build
   - support of parallel arcs in digraphs, see also: http://lemon.cs.elte.hu/pub/tutorial/a00011.html#sec_digraphs

Further libraries are available, but have not been integrated:
 - igraph: http://igraph.sourceforge.net/
   - C API
 - graph-tool: http://graph-tool.skewed.de/
   - boost graph based, with Python integration
   - autotools based build, very high compilation requirements
 - odgf: http://ogdf.net/doku.php
   - layout algorithms

Further resources:
 * http://stackoverflow.com/questions/2751826/which-c-graph-library-should-i-use
 * http://stackoverflow.com/questions/3010805/scalable-parallel-large-graph-analysis-library

## Motivation
A number of different graph libraries does exist - as listed above - though they cannot always be compared in a one to one fashion.
This library tries to provide a standard interface while maintaining access to
the particular feature of one library.
This allows to benefit from complementary graph algorithm implementations.
The goal of this library is to maintain a consistent and capable high-level interface abstraction in order to allow graph modeling and analysis.

Hence, the graph analysis library comes with a small framework to design
graph-based UI in QT.

## Examples

### Standard API Usage

```
#include "GraphAnalysis.hpp"
#include "lemon/Graph.hpp"
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
```

### Graph I/O

Please refer to the detailed description of graph_analysis::io::GraphIO which is part of the doxygen
documentation.

## Installation

To see the documentation of this project please do the following after
checkout to generate the doxygen-base documentation:

```
    mkdir build
    cd build
    make doc
```

Open the doxygen documentation with a browser: build/doc/index.html
The doxygen documentation contains all information on how to start
using the library along with the general API documentation.

## Merge Request and Issue Tracking

Gitlab is used for pull request and issue tracking: https://git.hb.dfki.de/dfki-libraries/graph_analysis/issues

## License

This project is licensed under the LGPL-2.1 or newer.

## Copyright

Copyright (c) 2013-2018 Thomas M. Roehr, DFKI GmbH Robotics Innovation Center
