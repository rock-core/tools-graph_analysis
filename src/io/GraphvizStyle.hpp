#ifndef GRAPH_ANALYSIS_IO_GRAPHVIZ_STYLE_HPP
#define GRAPH_ANALYSIS_IO_GRAPHVIZ_STYLE_HPP

#include "../BaseGraph.hpp"

namespace graph_analysis {
namespace io {

class GVGraph;

/**
 * \class GraphvizStyle
 * \brief A styling interface class for the GraphvizWriter
 * \see GraphvizWriter for further usage
 */
class GraphvizStyle
{
public:
    typedef shared_ptr<GraphvizStyle> Ptr;

    GraphvizStyle() {}
    virtual ~GraphvizStyle() {}

    /**
     * Apply style to an edge, e.g. by implementing the style
     *
     \verbatim
     gvGraph->setAttribute(edge,"color","red");
     \endverbatim
     */
    virtual void apply(const Edge::Ptr& edge, GVGraph* gvGraph, const BaseGraph::Ptr& graph) const { throw std::runtime_error("graph_analysis::io::GraphvizStyle::styleEdge"); }

    /**
     * Apply style to a vertex
     \verbatim
     gvGraph->setAttribute(vertex,"color","blue");
     \endverbatim
     */
    virtual void apply(const Vertex::Ptr& vertex, GVGraph* gvGraph, const BaseGraph::Ptr& graph) const { throw std::runtime_error("graph_analysis::io::GraphvizStyle::styleVertex"); }
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GRAPHVIZ_STYLE_HPP
