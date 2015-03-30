/**
 * \namespace percolation
 * \brief Percolation deals with a changing graph, i.e., nodes and edges that
 *        disappear from the graph
 */

namespace graph_analysis {
namespace percolation {

class Percolation
{
protected:
    DirectedGraph* mpGraph;

public:
    Percolation(BaseGraph* graph);

    virtual ~Percolation();
};

} // end namespace percolation
} // end namespace graph_analysis
