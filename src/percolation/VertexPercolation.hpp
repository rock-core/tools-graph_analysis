#ifndef GRAPH_ANALYSIS_PRECOLATION_VERTEX_PERCOLATION_HPP
#define GRAPH_ANALYSIS_PRECOLATION_VERTEX_PERCOLATION_HPP

namespace graph_analysis {
namespace percolation {

class VertexPercolation
{
public:
    VertexPercolation(BaseGraph* graph);

};

// Nomenclature according to: "Networks - An Introduction, M.E.J.Newton, p.593"
typedef VertexPercolation SitePercolation;

} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PRECOLATION_VERTEX_PERCOLATION_HPP
