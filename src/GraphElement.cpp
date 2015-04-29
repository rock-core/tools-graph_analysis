#include "GraphElement.hpp"
#include <limits>
#include <sstream>

namespace graph_analysis {

GraphElementId GraphElement::msUid = 0;

GraphElement::GraphElement()
    : mUid(msUid++)
{
    if(msUid == std::numeric_limits<GraphElementId>::max() )
    {
        throw std::runtime_error("GraphElement: maximum of GraphElements reached");
    }
}

/**
 * Get id of this element within a given graph
 */
GraphElementId GraphElement::getId(GraphId graphId) const
{
    typename GraphElementMap::const_iterator cit = mGraphElementMap.find(graphId);
    if( cit != mGraphElementMap.end())
    {
        return cit->second;
    }

    std::stringstream ss;
    ss << "GraphElement: this graph element is not part of the given graph (id:" << graphId << ")";
    throw std::runtime_error(ss.str());
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

std::string GraphElement::toPrefixedString(GraphId graph) const
{
    std::stringstream ss;
    ss << getId(graph);
    ss << ": ";
    ss << toString();
    return ss.str();
}

}
