#ifndef GRAPH_ANALYSIS_EDGE_PROPERTY_HPP
#define GRAPH_ANALYSIS_EDGE_PROPERTY_HPP

#include <vector>
#include <map>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

namespace graph_analysis {

/**
 * \brief A EdgeProperty can be associated with a edge to store additional data.
 * \details For the purpose of storing data with a edge, the storage type needs to inherit
 * from this class
 * \tparam GraphType The GraphType the edge belongs to
 * \tparam EdgeType The edge type the property will be associated with
 */
template<typename GraphType, typename EdgeType>
class EdgeProperty
{
public:
    typedef boost::shared_ptr< EdgeProperty<GraphType, EdgeType> > Ptr;
    typedef std::map<const GraphType*, EdgeType> GraphEdgeMap;
    typedef std::vector<const GraphType*> GraphList;

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::EdgeProperty"; }

    /**
     * Add the edge that corresponds to this propery in a given graph
     * This allows reverse mapping from the property to the graphs it belongs to
     */
    void addEdge(const GraphType* graph, const EdgeType& edge)
    {
        mGraphEdgeMap[graph] = edge;
    }

    /**
     * Remove the edge that corresponds to a given graph
     */
    void removeEdge(const GraphType* graph)
    {
        mGraphEdgeMap.erase(graph);
    }

    /**
     * Get edge
     * \param Graph to resolve the edge for this Property
     * \return edge in the graph
     * \throws if the EdgeProperty is not associated with the selected graph
     */
    EdgeType getEdge(const GraphType* graph) const
    {
        typename GraphEdgeMap::const_iterator cit = mGraphEdgeMap.find(graph);
        if( cit != mGraphEdgeMap.end())
        {
            return cit->second;
        }

        throw std::runtime_error("EdgeProperty: this edge property is not part of the given graph");
    }

    GraphList getGraphAssociations() const
    {
        GraphList graphList;
        typename GraphEdgeMap::const_iterator cit = mGraphEdgeMap.begin();
        for(; cit != mGraphEdgeMap.end(); ++cit)
        {
            graphList.push_back(cit->first);
        }

        return graphList;
    }

private:
    GraphEdgeMap mGraphEdgeMap;

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_PROPERTY_HPP
