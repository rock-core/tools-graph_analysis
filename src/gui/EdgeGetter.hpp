#ifndef GRAPH_ANALYSIS_EDGE_GETTER_HPP
#define GRAPH_ANALYSIS_EDGE_GETTER_HPP

#include <graph_analysis/Edge.hpp>

namespace graph_analysis {

class EdgeGetter
{
public:
    virtual ~EdgeGetter() {}
    virtual Edge::Ptr getEdge() const { throw std::runtime_error("graph_analysis::EdgeGetter::getEdge is not implemented"); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_GETTER_HPP
