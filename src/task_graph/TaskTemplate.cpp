#include "TaskTemplate.hpp"
#include "InstanceOf.hpp"
#include "Task.hpp"

#include <graph_analysis/io/OrogenModelReader.hpp>

#include <sstream>

namespace graph_analysis
{
namespace task_graph
{
TaskTemplate::TaskTemplate(const std::string& label)
    : Vertex(label)
{
}

void TaskTemplate::loadFromFile(const std::string& yamlFileName)
{
    // Create a new BaseGraph
    mpBaseGraph = BaseGraph::getInstance();

    // Fill the BaseGraph from yaml file
    io::OrogenModelReader reader;
    reader.read(yamlFileName, mpBaseGraph);

    // Find root (Task) vertex
    VertexIterator::Ptr it = mpBaseGraph->getVertexIterator();
    while(it->next())
    {
        if(it->current()->getClassName() == Task::vertexType())
        {
            mpRootVertex = dynamic_pointer_cast<Task>(it->current());
            break;
        }
    }

    // Store the yaml file name
    mYamlFileName = yamlFileName;
    mLabel = mpRootVertex->getLabel();
    mInstance = 0;
}

std::string searchAndReplace(const std::string& original, const std::string& token, const std::string& replacement)
{
    std::string newStr("");
    std::size_t pos;
    std::size_t lastPos = 0;
    while ((pos = original.find(token, lastPos)) != std::string::npos)
    {
        newStr += original.substr(lastPos, pos);
        newStr += replacement;
        lastPos = pos + token.length();
    }
    newStr += original.substr(lastPos, std::string::npos);
    return newStr;
}

TaskPtr TaskTemplate::instantiateAndAddTo(BaseGraph::Ptr graph,
                                          const std::string& label)
{
    // Clone the graph
    BaseGraph::Ptr copy = mpBaseGraph->clone();

    // Find root (Task) vertex
    Task::Ptr newRootVertex;
    VertexIterator::Ptr it = copy->getVertexIterator();
    while(it->next())
    {
        if(it->current()->getClassName() == Task::vertexType())
        {
            newRootVertex = dynamic_pointer_cast<Task>(it->current());
            break;
        }
    }

    // Label the root vertex
    mInstance++;
    std::stringstream ss;
    ss << mInstance;
    if(label.empty())
    {
        // When we use the default label we have to
        // get rid of any '::' and replace them by '-'
        std::string fixed(searchAndReplace(mLabel + ss.str(), "::", "-"));
        newRootVertex->setLabel(fixed);
    }
    else
    {
        newRootVertex->setLabel(label);
    }

    // Add all vertices and edges to the target graph
    VertexIterator::Ptr vit = copy->getVertexIterator();
    while(vit->next())
    {
        graph->addVertex(vit->current());
    }
    EdgeIterator::Ptr eit = copy->getEdgeIterator();
    while(eit->next())
    {
        graph->addEdge(eit->current());
    }

    // Find me in the given graph ...
    Vertex::Ptr myself = getSharedPointerFromGraph(graph);
    // Find the new rootVertex in the given graph
    Vertex::Ptr mychild = newRootVertex->getSharedPointerFromGraph(graph);

    // ... and finally link myself and my instance
    // NOTE: myChild is a instance-of mySelf :D
    InstanceOf::Ptr relation =
        InstanceOf::Ptr(new InstanceOf(mychild, myself, ss.str()));
    graph->addEdge(relation);

    return newRootVertex;
}

const BaseGraph::Ptr& TaskTemplate::getInternalBaseGraph() const
{
    return mpBaseGraph;
}
}
}
