#include "GraphElement.hpp"

namespace graph_analysis {

/**
 * Get id of this element within a given graph
 */
GraphElementId GraphElement::getId(GraphId graph) const
{
    typename GraphElementMap::const_iterator cit = mGraphElementMap.find(graph);
    if( cit != mGraphElementMap.end())
    {
        return cit->second;
    }

    throw std::runtime_error("GraphElement: this graph element is not part of the given graph");
}

GraphIdList GraphElement::getGraphAssociations() const
{
    GraphIdList graphList;
    typename GraphElementMap::const_iterator cit = mGraphElementMap.begin();
    for(; cit != mGraphElementMap.end(); ++cit)
    {
        graphList.push_back(cit->first);
    }

    return graphList;
}

}
