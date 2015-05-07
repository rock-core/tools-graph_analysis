#ifndef GRAPH_ANALYSIS_IO_GEXF_READER_HPP
#define GRAPH_ANALYSIS_IO_GEXF_READER_HPP

namespace graph_analysis {
namespace io {

class GexfReader : public Reader
{
public:
    void read(const std::string& filename, const BaseGraph::Ptr& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GEXF_READER_HPP
