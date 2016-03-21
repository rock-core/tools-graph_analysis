#ifndef GRAPH_ANALYSIS_ALGORITHMS_DELTA_HPP
#define GRAPH_ANALYSIS_ALGORITHMS_DELTA_HPP

#include <graph_analysis/BaseGraph.hpp>

namespace graph_analysis {
namespace algorithms {

/**
 * \brief Computes differences between two graphs
 * \details 
 * A Delta object takes two graphs as arguments and finds the steps
 * to get from the first graph to the second one.
 * Uses removeVertices/addVertices/removeEdges/addEdges vectors to store 
 * the vertices/edges that needs to be added/removed from the first graph
 * to get the second one. 
 * Need to run the 'run' method before the getNext method.
 */
class Delta
{
public:
    Delta(const BaseGraph::Ptr& graph1, const BaseGraph::Ptr& graph2);

	/**
	 * check differences between the given graphs (mpSourceGraph and mpTargetGraph) 
	 * returns the steps to get from firstGraph to secondGraph into the vectors defined as properties
     */
    void run();

    /**
	 * getNext will execute only one operation when is applied
	 * The order of opertations will be:
	 * - removes all edges that has to be removed from mpSourceGraph (until removeEdges is empty)
	 * - removes all vertices that has to be removed from mpSourceGraph (until removeVertices is empty)
	 * - adds all vertices that has to be added in mpSourceGraph (until addVertices is empty)
	 * - adds all edges that has to be added in mpSourceGraph (until addEdges is empty)
	 * !It will execute only one removal or add-on when is called and it will return the resulting graph after the modification
	 * To get the second graph, this function should be called until all vectors are empty.  
     */
    BaseGraph::Ptr getNext();

    /** 
     * check if the source graph has a next
     * if the size of at least one of the vectors is bigger than 0 => true
     * if all sizes are 0 => false
     */
    bool hasNext();

private:
    BaseGraph::Ptr mpSourceGraph;
    BaseGraph::Ptr mpTargetGraph;
    
    std::vector<Vertex::Ptr> mRemoveVertices;
    std::vector<Vertex::Ptr> mAddVertices;
    std::vector<Edge::Ptr> mRemoveEdges;
    std::vector<Edge::Ptr> mAddEdges;
};

} // end namespace algorithms
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_ALGORITHMS_DELTA_HPP