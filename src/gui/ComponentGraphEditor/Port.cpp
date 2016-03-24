#include "Port.hpp"

#include "HasFeature.hpp"
#include <graph_analysis/gui/ComponentGraphEditor/Component.hpp>

namespace graph_analysis {

Port::Port(const std::string& label)
    : Vertex(label)
{}

shared_ptr<Component> Port::getComponent(const BaseGraph::Ptr &graph) const
{
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getInEdgeIterator(vertexPtr);
    while(edgeIt->next())
    {
        graph_analysis::HasFeature::Ptr feature =
            dynamic_pointer_cast<HasFeature>(edgeIt->current());
        if(!feature)
        {
            continue;
        }
        graph_analysis::Component::Ptr comp =
            dynamic_pointer_cast<Component>(feature->getSourceVertex());
        if(comp)
        {
            return comp;
        }
    }
    return Component::Ptr();
}

} // end namespace graph_analysis
