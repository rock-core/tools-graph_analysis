#include "CndModelReader.hpp"

#include <graph_analysis/task_graph/HasFeature.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/task_graph/Task.hpp>
#include <yaml-cpp/yaml.h>

namespace graph_analysis
{
namespace io
{

task_graph::Task::Ptr getTaskByLabel(BaseGraph::Ptr graph,
                                     const std::string& name)
{
    VertexIterator::Ptr it = graph->getVertexIterator();
    while(it->next())
    {
        if(it->current()->getClassName() == task_graph::Task::vertexType())
            if(it->current()->getLabel() == name)
                return dynamic_pointer_cast<task_graph::Task>(it->current());
    }
    return task_graph::Task::Ptr();
}

// FIXME: This should be a member function of a Task/Task Template
task_graph::OutputPort::Ptr getOutputByLabel(BaseGraph::Ptr graph,
                                             task_graph::Task::Ptr task,
                                             const std::string& name)
{
    std::vector<task_graph::OutputPort::Ptr> outputs =
        task->getOutputPorts(graph);
    std::vector<task_graph::OutputPort::Ptr>::const_iterator it;

    for(it = outputs.begin(); it != outputs.end(); ++it)
        if((*it)->getLabel() == name)
            return (*it);
    return task_graph::OutputPort::Ptr();
}

// FIXME: This should be a member function of a Task/Task Template
task_graph::InputPort::Ptr getInputByLabel(BaseGraph::Ptr graph,
                                           task_graph::Task::Ptr task,
                                           const std::string& name)
{
    std::vector<task_graph::InputPort::Ptr> inputs = task->getInputPorts(graph);
    std::vector<task_graph::InputPort::Ptr>::const_iterator it;

    for(it = inputs.begin(); it != inputs.end(); ++it)
        if((*it)->getLabel() == name)
            return (*it);
    return task_graph::InputPort::Ptr();
}

// TODO: There should be a task constructor/member function to add ports?!
template <typename T>
typename T::Ptr createPort(const std::string& label, BaseGraph::Ptr graph,
                           task_graph::Task::Ptr parent)
{
    typename T::Ptr port = typename T::Ptr(new T(label));
    graph->addVertex(port);
    task_graph::HasFeature::Ptr has = task_graph::HasFeature::Ptr(
        new task_graph::HasFeature(parent, port, "has"));
    graph->addEdge(has);
    return port;
}

void CndModelReader::read(const std::string& filename, BaseGraph::Ptr graph)
{
    // Destroy old graph if necessary
    graph->clear();

    YAML::Node network = YAML::LoadFile(filename);
    YAML::Node tasks(network["tasks"]);
    YAML::Node connections(network["connections"]);

    YAML::const_iterator it;
    // Create task nodes
    for(it = tasks.begin(); it != tasks.end(); ++it)
    {
        // std::cout << "TASK: " << it->first.as<std::string>() << std::endl;

        std::string label = it->first.as<std::string>();
        std::string templ = it->second["type"].as<std::string>();
        task_graph::Task::Ptr comp =
            task_graph::Task::Ptr(new task_graph::Task(templ,label));
        graph->addVertex(comp);
    }

    // Create task interconnection (ports will be created on demand)
    for(it = connections.begin(); it != connections.end(); ++it)
    {
        // std::cout << "CONNECTION: " << it->first.as<std::string>() <<
        // std::endl;

        std::string label = it->second["transport"].as<std::string>();
        YAML::Node from(it->second["from"]);
        YAML::Node to(it->second["to"]);
        task_graph::Task::Ptr sourceTask =
            getTaskByLabel(graph, from["task_id"].as<std::string>());
        task_graph::Task::Ptr targetTask =
            getTaskByLabel(graph, to["task_id"].as<std::string>());
        // std::cout << sourceTask->getLabel() << " -> " <<
        // targetTask->getLabel() << std::endl;
        task_graph::OutputPort::Ptr output = getOutputByLabel(
            graph, sourceTask, from["port_name"].as<std::string>());
        if(!output)
            output = createPort<task_graph::OutputPort>(
                from["port_name"].as<std::string>(), graph, sourceTask);
        task_graph::InputPort::Ptr input = getInputByLabel(
            graph, targetTask, to["port_name"].as<std::string>());
        if(!input)
            input = createPort<task_graph::InputPort>(
                to["port_name"].as<std::string>(), graph, targetTask);
        // std::cout << output->getLabel() << " -> " << input->getLabel() <<
        // std::endl;
        task_graph::PortConnection::Ptr conn = task_graph::PortConnection::Ptr(
            new task_graph::PortConnection(output, input, label));
        graph->addEdge(conn);
    }

    // TODO: Shall we compare/match instantiated tasks with task templates and
    // warn if they dont fit?
    // TODO: Add checks 'n' stuff
    // TODO: Add other stuff/parameters ...
}
}
}
