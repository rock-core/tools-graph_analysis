#include "OrogenModelReader.hpp"
#include <graph_analysis/VertexTypeManager.hpp>

namespace graph_analysis {
namespace io {

void OrogenModelReader::read(const std::string &filename,
                             BaseGraph::Ptr graph) {
    std::cout << "OrogenModelReader called with " << filename << "\n";
}

} // end namespace io
} // end namespace graph_analysis
