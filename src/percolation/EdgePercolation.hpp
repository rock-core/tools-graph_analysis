#ifndef GRAPH_ANALYSIS_PRECOLATION_EDGE_PERCOLATION_HPP
#define GRAPH_ANALYSIS_PRECOLATION_EDGE_PERCOLATION_HPP

namespace graph_analysis {
namespace percolation {

/**
 * \class EdgePercolation
 * \brief Class to simulate edge percolation on a graph
 */
class EdgePercolation : public Percolation
{
public:
    EdgePercolation(DirectedGraph* graph);

    /**
     * Mark an edge as present
     */
    virtual void markAsOccupied(Edge::Ptr edge);

    /**
     * Mark an edge as not occupied
     */
    virtual void markAsNotOccupied(Edge::Ptr edge);

};

// Nomenclature according to: "Networks - An Introduction, M.E.J.Newton, p.593"
typedef EdgePercolation BondPercolation;

} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PRECOLATION_EDGE_PERCOLATION_HPP
