#include "Property.hpp"
#include "HasFeature.hpp"
#include "HasUniqueFeature.hpp"
#include <iostream>
#include <cstdlib>

namespace graph_analysis
{
namespace task_graph
{

Property::Property(const std::string& label)
    : Vertex(label)
{
}

Vertex::Ptr Property::getOwner(const BaseGraph::Ptr& graph) const
{
    Vertex::Ptr owner;
    Vertex::Ptr me = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr eit = graph->getInEdgeIterator(me);
    while(eit->next())
    {
        HasFeature::Ptr edge = dynamic_pointer_cast<HasFeature>(eit->current());
        if(!edge)
            continue;

        if(owner)
            throw std::runtime_error(
                "Property::getOwner: Multiple owners detected");

        owner = edge->getSourceVertex();
    }

    return owner;
}

DataType::Ptr Property::getType(const BaseGraph::Ptr& graph) const
{
    DataType::Ptr type;
    Vertex::Ptr me = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr eit = graph->getOutEdgeIterator(me);
    while(eit->next())
    {
        HasUniqueFeature::Ptr edge =
            dynamic_pointer_cast<HasUniqueFeature>(eit->current());
        if(!edge)
            continue;

        DataType::Ptr vertex =
            dynamic_pointer_cast<DataType>(edge->getTargetVertex());
        if(!vertex)
            continue;

        if(type)
            throw std::runtime_error(
                "Property::getType: Multiple types detected");
        type = vertex;
    }

    return type;
}

DataValue::Ptr Property::getValue(const BaseGraph::Ptr& graph) const
{
    DataValue::Ptr value;
    Vertex::Ptr me = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr eit = graph->getOutEdgeIterator(me);
    while(eit->next())
    {
        HasUniqueFeature::Ptr edge =
            dynamic_pointer_cast<HasUniqueFeature>(eit->current());
        if(!edge)
            continue;

        DataValue::Ptr vertex =
            dynamic_pointer_cast<DataValue>(edge->getTargetVertex());
        if(!vertex)
            continue;

        if(value)
            throw std::runtime_error(
                "Property::getValue: Multiple values detected");
        value = vertex;
    }

    return value;
}

bool Property::isNumeric() const
{
    int blob;
    return isNumeric(&blob);
}

bool Property::isNumeric(int *value) const
{
    const char *label = getLabel().c_str();
    char *str_end;

    *value = strtol(label, &str_end, 10);

    // If invalid (no-digit) characters have been found we have a NON purely numeric label
    if (*str_end != '\0')
        return false;
    return true;
}

}
}
