#include "Task.hpp"
#include "HasFeature.hpp"
#include "InstanceOf.hpp"
#include "TaskTemplate.hpp"

namespace graph_analysis
{
namespace task_graph
{

Task::Task(const std::string& label)
    : Vertex(label)
{
}

TaskTemplatePtr Task::getTemplate(const BaseGraph::Ptr& graph) const
{
    TaskTemplatePtr myParent;
    // obtain the shared pointer of "this"...
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr eit = graph->getOutEdgeIterator(vertexPtr);
    while(eit->next())
    {
        if(eit->current()->getClassName() != InstanceOf::edgeType())
            continue;

        if(myParent)
            throw std::runtime_error(
                "Task::getTemplate: Multiple parents detected");

        myParent = dynamic_pointer_cast<TaskTemplate>(
            eit->current()->getTargetVertex());
    }

    return myParent;
}

std::vector<OutputPort::Ptr>
Task::getOutputPorts(const BaseGraph::Ptr& graph) const
{
    std::vector<OutputPort::Ptr> retval;
    // obtain the shared pointer of "this"...
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getOutEdgeIterator(vertexPtr);
    while(edgeIt->next())
    {
        HasFeature::Ptr feature =
            dynamic_pointer_cast<HasFeature>(edgeIt->current());
        if(!feature)
        {
            continue;
        }

        // creating new node items
        OutputPort::Ptr port =
            dynamic_pointer_cast<OutputPort>(feature->getTargetVertex());
        if(!port)
        {
            continue;
        }

        retval.push_back(port);
    }
    return retval;
}

std::vector<InputPort::Ptr>
Task::getInputPorts(const BaseGraph::Ptr& graph) const
{
    std::vector<InputPort::Ptr> retval;
    // obtain the shared pointer of "this"...
    Vertex::Ptr vertexPtr = getSharedPointerFromGraph(graph);
    EdgeIterator::Ptr edgeIt = graph->getOutEdgeIterator(vertexPtr);
    while(edgeIt->next())
    {
        HasFeature::Ptr feature =
            dynamic_pointer_cast<HasFeature>(edgeIt->current());
        if(!feature)
        {
            continue;
        }

        // creating new node items
        InputPort::Ptr port =
            dynamic_pointer_cast<InputPort>(feature->getTargetVertex());
        if(!port)
        {
            continue;
        }

        retval.push_back(port);
    }
    return retval;
}
}
} // end namespace graph_analysis
