#ifndef GRAPH_ANALYSIS_PERCOLATION_EVENT_HPP
#define GRAPH_ANALYSIS_PERCOLATION_EVENT_HPP

#include <vector>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis {
namespace percolation {

class Event
{
public:
    enum Type { ADD_VERTEX, REMOVE_VERTEX, ADD_EDGE, REMOVE_EDGE };

    Event(Type type, GraphElement::Ptr element);

private:
    /// Change item
    Type mType;
    GraphElement::Ptr mElement;
};

typedef std::vector<Event> EventList;

} // end namespace percolation
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PERCOLATION_EVENT_HPP
