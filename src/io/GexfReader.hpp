#ifndef GRAPH_ANALYSIS_IO_GEXF_READER_HPP
#define GRAPH_ANALYSIS_IO_GEXF_READER_HPP

#include <map>
#include <libgexf/gexf.h>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Edge.hpp>

namespace graph_analysis {
namespace io {

class GexfReader : public Reader
{
    typedef std::map < libgexf::t_id, Vertex::Ptr > VertexMap;
public:
    void read(const std::string& filename, const BaseGraph::Ptr& graph) const;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GEXF_READER_HPP
