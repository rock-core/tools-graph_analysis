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
    BaseGraph* mpGraph;

    /**
     * Marks an element as occupied, i.e., being in the graph
     * Alias method for markAsPresent
     * \param id Id of the graph element (edge or vertex)
     */
    void markAsOccupied(GraphElementId id) { markAsPresent(id); }

    /**
     * Marks an element as unoccupied, i.e., not being in the graph
     * Alias method for markAsMissing
     * \param id Id of the graph element (edge or vertex)
     */
    void markAsUnoccupied(GraphElementId id) { markAsMissing(id); }

    /**
     * Marks an element as present in the graph
     * \param id Id of the graph element (edge or vertex)
     * \see markAsOccupied
     */
    virtual void markAsPresent(GraphElementId id) { throw std::runtime_error("graph_analysis::percolation::Percolation::markAsPresent has not been implemented"); }

    /**
     * Marks an element as not present in the graph
     * \param id Id of the graph element (edge or vertex)
     */
    virtual void markAsNotPresent(GraphElementId id) { throw std::runtime_error("graph_analysis::percolation::Percolation::markAsNotPresent has not been implemented"); }

public:
    Percolation(BaseGraph* graph);

    virtual ~Percolation();
};

} // end namespace percolation
} // end namespace graph_analysis
