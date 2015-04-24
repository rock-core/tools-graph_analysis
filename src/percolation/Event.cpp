#include "Event.hpp"

namespace graph_analysis {
namespace percolation {

Event::Event(Event::Type type, GraphElement::Ptr element)
    : mType(type)
    , mElement(element)
{}

} // end namespace percolation
} // end namespace graph_analysis
