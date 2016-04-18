#include "CndModelReader.hpp"

#include <graph_analysis/task_graph/DataType.hpp>
#include <graph_analysis/task_graph/DataValue.hpp>
#include <graph_analysis/task_graph/HasFeature.hpp>
#include <graph_analysis/task_graph/HasUniqueFeature.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/task_graph/Property.hpp>
#include <graph_analysis/task_graph/Task.hpp>
#include <graph_analysis/task_graph/TaskTemplate.hpp>
#include <yaml-cpp/yaml.h>

namespace graph_analysis
{
namespace io
{

// FIXME: This should be a member of BaseGraph
template <typename T>
typename T::Ptr getVertexByLabel(const std::string& label, BaseGraph::Ptr graph)
{
    VertexIterator::Ptr it = graph->getVertexIterator();
    while(it->next())
    {
        if(it->current()->getClassName() == T::vertexType())
            if(it->current()->getLabel() == label)
                return dynamic_pointer_cast<T>(it->current());
    }
    return typename T::Ptr();
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

task_graph::Property::Ptr getPropertyByLabel(BaseGraph::Ptr graph,
                                             Vertex::Ptr vertex,
                                             const std::string& label)
{
    task_graph::Property::Ptr prop;
    EdgeIterator::Ptr eit = graph->getOutEdgeIterator(vertex);
    while(eit->next())
    {
        // Check type
        if(eit->current()->getClassName() != task_graph::HasFeature::edgeType())
            continue;

        // Check if target is a property
        Vertex::Ptr target = eit->current()->getTargetVertex();
        if(target->getClassName() != task_graph::Property::vertexType())
            continue;

        if(target->getLabel() != label)
            continue;

        prop = dynamic_pointer_cast<task_graph::Property>(target);
    }
    return prop;
}

void readPropertiesRecursively(YAML::Node& node, Vertex::Ptr vertex,
                               BaseGraph::Ptr graph)
{
    switch(node.Type())
    {
    case YAML::NodeType::Scalar:
    {
        // Search for a data value vertex
        task_graph::Property::Ptr prop =
            dynamic_pointer_cast<task_graph::Property>(vertex);
        task_graph::DataValue::Ptr dvalue = prop->getValue(graph);
        if(!dvalue)
        {
            // If not found, create data value
            dvalue = task_graph::DataValue::Ptr(
                new task_graph::DataValue(node.as<std::string>()));
            graph->addVertex(dvalue);
            // Link data value to property
            task_graph::HasUniqueFeature::Ptr hasU2 =
                task_graph::HasUniqueFeature::Ptr(
                    new task_graph::HasUniqueFeature(vertex, dvalue,
                                                     "has-unique"));
            graph->addEdge(hasU2);
        }
        else
        {
            // If found, update value
            dvalue->setLabel(node.as<std::string>());
        }
        break;
    }
    case YAML::NodeType::Sequence:
    case YAML::NodeType::Map:
    {
        // std::cout << "CONTAINER " << node.as<std::string>() << std::endl;
        YAML::const_iterator it;
        for(it = node.begin(); it != node.end(); ++it)
        {
            // Search for the property
            task_graph::Property::Ptr prop =
                getPropertyByLabel(graph, vertex, it->first.as<std::string>());
            // Create property if not found
            if(!prop)
            {
                prop = task_graph::Property::Ptr(
                    new task_graph::Property(it->first.as<std::string>()));
                graph->addVertex(prop);
                // Link property to parent
                task_graph::HasFeature::Ptr has = task_graph::HasFeature::Ptr(
                    new task_graph::HasFeature(vertex, prop, "has"));
                graph->addEdge(has);
            }
            // This node may contain other properties, so dig deeper
            YAML::Node next(it->second);
            readPropertiesRecursively(next, prop, graph);
        }
        break;
    }
    default:
    {
        std::cout << "W00t?!";
        break;
    }
    }
}

void CndModelReader::read(const std::string& filename, BaseGraph::Ptr graph)
{
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

        // Find the template in given graph
        task_graph::TaskTemplate::Ptr parent =
            getVertexByLabel<task_graph::TaskTemplate>(templ, graph);
        if(!parent)
        {
            throw std::runtime_error(
                "CndModelReader::read(): Could not find template '" + templ +
                "'");
            return;
        }
        // Then instantiate from template
        task_graph::Task::Ptr task = parent->instantiateAndAddTo(graph, label);

        // Now we have to import all properties (excluding "type")
        YAML::Node next(it->second);
        readPropertiesRecursively(next, task, graph);
    }

    // Create task interconnection (ports will be created on demand)
    for(it = connections.begin(); it != connections.end(); ++it)
    {
        // std::cout << "CONNECTION: " << it->first.as<std::string>() <<
        // std::endl;

        std::string label = it->first.as<std::string>();
        YAML::Node from(it->second["from"]);
        YAML::Node to(it->second["to"]);

        task_graph::Task::Ptr sourceTask = getVertexByLabel<task_graph::Task>(
            from["task_id"].as<std::string>(), graph);
        task_graph::Task::Ptr targetTask = getVertexByLabel<task_graph::Task>(
            to["task_id"].as<std::string>(), graph);

        task_graph::OutputPort::Ptr output = getOutputByLabel(
            graph, sourceTask, from["port_name"].as<std::string>());

        task_graph::InputPort::Ptr input = getInputByLabel(
            graph, targetTask, to["port_name"].as<std::string>());

        // TODO: Check if ports can be connected (have to have the same DATA
        // TYPE)
        task_graph::PortConnection::Ptr conn = task_graph::PortConnection::Ptr(
            new task_graph::PortConnection(output, input, label));
        graph->addEdge(conn);

        // TODO: Add properties. For this we need the concept of a CONNECTOR
    }

    // TODO: Add checks 'n' stuff
    // TODO: Add other stuff/parameters ...
}
}
}
