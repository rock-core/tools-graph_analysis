#include "Task.hpp"
#include "HasFeature.hpp"

//#include <base/Logging.hpp>

namespace graph_analysis
{
namespace task_graph {

Task::Task(const std::string &label) : Vertex(label) {}
Task::Task(const std::string &templateLabel, const std::string &label) : Vertex(label), mTemplateLabel(templateLabel) {}

std::string Task::getTemplateLabel() const
{
    return mTemplateLabel;
}

void Task::setTemplateLabel (const std::string& templateLabel)
{
    mTemplateLabel = templateLabel;
}

std::vector<OutputPort::Ptr>
Task::getOutputPorts(const BaseGraph::Ptr &graph) const
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
Task::getInputPorts(const BaseGraph::Ptr &graph) const
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
