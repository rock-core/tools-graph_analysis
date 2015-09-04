#ifndef GRAPH_ANALYSIS_GUI_VERTEX_GETTER_HPP
#define GRAPH_ANALYSIS_GUI_VERTEX_GETTER_HPP

#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

class VertexGetter
{
public:
    virtual ~VertexGetter() {}
    virtual Vertex::Ptr getVertex() const { throw std::runtime_error("graph_analysis::VertexGetter::getVertex is not implemented"); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_VERTEX_GETTER_HPP
