#ifndef GRAPH_ANALYSIS_IO_GEXF_WRITER_HPP
#define GRAPH_ANALYSIS_IO_GEXF_WRITER_HPP

#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis {
namespace io {

class GexfWriter : public Writer
{
public:
    void write(const std::string& filename, const BaseGraph::Ptr& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GEXF_WRITER_HPP

