#ifndef GRAPH_ANALYISIS_BIPARTITE_GRAPH_HPP
#define GRAPH_ANALYISIS_BIPARTITE_GRAPH_HPP

#include "BaseGraph.hpp"
#include "SubGraph.hpp"

namespace graph_analysis {

/**
 * \class BipartiteGraph
 * \brief Implementation of a bipartite graph, that consists of a set of 'blue'
 * nodes and of 'red' nodes.
 * \details For the implementation a general graph will store all vertices and
 * edges, while the two set of blue and red vertices will be identified through
 * two corresponding SubGraph instances
 *
 */
class BipartiteGraph
{
public:
    BipartiteGraph();

    /**
     * Link two vertices -- vertices will be added to the corresponding subsets
     * \param red Vertex that belongs to the red set
     * \param blue Vertex that belongs to the blue set
     * \param label Label that will be set on the edge(s) added to the graph
     */
    void linkVertices(const Vertex::Ptr& red, const Vertex::Ptr& blue, const std::string& label = "");

    /**
     * Get all vertex partner, i.e. vertices
     */
    std::vector<Vertex::Ptr> getPartners(const Vertex::Ptr& vertex) const;

    /**
     * Retrieve the single partner
     * \throw if there are more partner than 1
     */
    Vertex::Ptr getUniquePartner(const Vertex::Ptr& vertex) const;

    /**
     * Get the subgraph that contains only red vertices
     * \return SubGraph
     */
    SubGraph::Ptr getRedSubGraph() const { return mpRed; }

    /**
     * Get the subgraph that contains only blue vertices
     * \return SubGraph
     */
    SubGraph::Ptr getBlueSubGraph() const { return mpBlue; }

protected:
private:
    BaseGraph::Ptr mpGraph;

    SubGraph::Ptr mpRed;
    SubGraph::Ptr mpBlue;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYISIS_BIPARTITE_GRAPH_HPP
