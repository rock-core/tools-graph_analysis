#include "GraphElement.hpp"
#include <limits>
#include <sstream>

namespace graph_analysis {

boost::uuids::random_generator GraphElement::msUuidGenerator;
std::map<GraphElementUuid, function<GraphElement::Ptr()> > GraphElement::msGraphElements;

GraphElement::GraphElement(const std::string& label)
    : mUuid(msUuidGenerator())
    , mLabel(label)
{
    msGraphElements[mUuid] = bind(&GraphElement::getSharedFromThis,this);
}

GraphElement::~GraphElement()
{
    msGraphElements.erase(mUuid);
}

/**
 * Get id of this element within a given graph
 */
GraphElementId GraphElement::getId(GraphId graphId) const
{
    GraphElementMap::const_iterator cit = mGraphElementMap.find(graphId);
    if( cit != mGraphElementMap.end())
    {
        return cit->second;
    }

    std::stringstream ss;
    ss << "GraphElement: this graph element '" << toString() << "' is not part of the given graph (id:" << graphId << ")";
    throw std::runtime_error(ss.str());
}


GraphElement::Ptr GraphElement::fromUuid(const GraphElementUuid& uuid)
{
    std::map<GraphElementUuid, function<GraphElement::Ptr()> >::iterator it = msGraphElements.find(uuid);
    if(it != msGraphElements.end())
    {
        return it->second();
    } else {
        std::stringstream ss;
        ss << uuid;
        throw std::invalid_argument("graph_analysis::GraphElement::fromUuid:"
                " could not find graph element with uuid: '" + ss.str() + "'");
    }
}

GraphIdList GraphElement::getGraphAssociations() const
{
    GraphIdList graphList;
    GraphElementMap::const_iterator cit = mGraphElementMap.begin();
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
