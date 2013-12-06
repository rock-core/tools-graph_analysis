#ifndef GRAPH_ANALYSIS_VERTEX_PROPERTY_HPP
#define GRAPH_ANALYSIS_VERTEX_PROPERTY_HPP

#include <vector>
#include <map>
#include <stdexcept>
#include <boost/shared_ptr.hpp>

namespace graph_analysis
{

/**
 * \brief A VertexProperty can be associated with a vertex to store additional data.
 * \details For the purpose of storing data with a vertex, the storage type needs to inherit
 * from this class
 * \tparam GraphType The GraphType the vertex belongs to
 * \tparam VertexType The vertex type the property will be associated with
 */
template<typename GraphType, typename VertexType>
class VertexProperty
{
public:
    typedef boost::shared_ptr< VertexProperty<GraphType, VertexType> > Ptr;
    typedef std::map<const GraphType*, VertexType> GraphVertexMap;
    typedef std::vector<const GraphType*> GraphList;

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::VertexProperty"; }

    /**
     * Add the vertex that corresponds to this propery in a given graph.
     * This allows reverse mapping from the property to the graphs it belongs to
     * \param vertex The vertex that this property is associated with
     */
    void addVertex(const GraphType* graph, const VertexType& vertex)
    {
        mGraphVertexMap[graph] = vertex;
    }

    /**
     * Remove the vertex that corresponds to a given graph
     */
    void removeVertex(const GraphType* graph)
    {
        mGraphVertexMap.erase(graph);
    }

    /**
     * Get vertex
     * \return vertex
     */
    VertexType getVertex(const GraphType* graph) const
    {
        typename GraphVertexMap::const_iterator cit = mGraphVertexMap.find(graph);
        if( cit != mGraphVertexMap.end())
        {
            return cit->second;
        }

        throw std::runtime_error("VertexProperty: this vertex property is not part of the given graph");
    }

    GraphList getGraphAssociations() const
    {
        GraphList graphList;
        typename GraphVertexMap::const_iterator cit = mGraphVertexMap.begin();
        for(; cit != mGraphVertexMap.end(); ++cit)
        {
            graphList.push_back(cit->first);
        }

        return graphList;
    }

private:
    GraphVertexMap mGraphVertexMap;
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_PROPERTY_HPP
