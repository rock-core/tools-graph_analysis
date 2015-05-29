#ifndef GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
#define GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP

#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis {
namespace io {

class GraphvizWriter : public Writer
{
    std::string mLayout;
public:
    GraphvizWriter(const std::string& layout = "dot");
    ~GraphvizWriter();

    void write(const std::string& filename, const BaseGraph& graph) const;

    void write(const std::string& filename, const BaseGraph::Ptr& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GRAPHVIZWRITER_HPP
