#include "CndModelWriter.hpp"

#include <fstream>
#include <graph_analysis/task_graph/DataType.hpp>
#include <graph_analysis/task_graph/DataValue.hpp>
#include <graph_analysis/task_graph/HasFeature.hpp>
#include <graph_analysis/task_graph/HasUniqueFeature.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/task_graph/Property.hpp>
#include <graph_analysis/task_graph/Task.hpp>
#include <graph_analysis/task_graph/TaskTemplate.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

namespace graph_analysis
{
namespace io
{

void writePropertiesRecursively(YAML::Node& node, const Vertex::Ptr& vertex,
                                const BaseGraph::Ptr& graph)
{
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

        // Write the property
        task_graph::Property::Ptr prop =
            dynamic_pointer_cast<task_graph::Property>(target);
        // std::string type = prop->getType(graph)->getLabel();
        std::string value = "";
        if(prop->getValue(graph))
        {
            value = prop->getValue(graph)->getLabel();
        }

        YAML::Node uerg(node[prop->getLabel()]);

        if(!value.empty())
            uerg = value;
        else
            // ... and search the child (TODO: How do we prevent loops?)
            writePropertiesRecursively(uerg, target, graph);
    }
}

void internal_write(YAML::Node& doc, const BaseGraph::Ptr& graph)
{
    // Cycle thorugh all vertices
    VertexIterator::Ptr vit = graph->getVertexIterator();
    while(vit->next())
    {
        // Check if vertex is a Task
        if(vit->current()->getClassName() != task_graph::Task::vertexType())
            continue;

        // Get task and the corresponding template
        task_graph::Task::Ptr task =
            dynamic_pointer_cast<task_graph::Task>(vit->current());
        task_graph::TaskTemplate::Ptr taskTemp = task->getTemplate(graph);

        // It is a task, so add the task to the list of tasks
        doc["tasks"][task->getLabel()]["type"] = taskTemp->getLabel();

        YAML::Node uerg(doc["tasks"][task->getLabel()]);

        // For all associated properties
        writePropertiesRecursively(uerg, task, graph);
    }

    // Cycle thorugh all edges
    EdgeIterator::Ptr eit = graph->getEdgeIterator();
    while(eit->next())
    {
        // Check if edge is a PortConnection
        if(eit->current()->getClassName() !=
           task_graph::PortConnection::edgeType())
            continue;

        task_graph::PortConnection::Ptr conn =
            dynamic_pointer_cast<task_graph::PortConnection>(eit->current());

        // Register the ports
        task_graph::OutputPort::Ptr output = conn->getSourcePort(graph);
        task_graph::InputPort::Ptr input = conn->getTargetPort(graph);
        doc["connections"][conn->getLabel()]["from"]["port_name"] =
            output->getLabel();
        doc["connections"][conn->getLabel()]["to"]["port_name"] =
            input->getLabel();

        // Find the corresponding source and target task vertices
        task_graph::Task::Ptr source = output->getTask(graph);
        task_graph::Task::Ptr target = input->getTask(graph);
        doc["connections"][conn->getLabel()]["from"]["task_id"] =
            source->getLabel();
        doc["connections"][conn->getLabel()]["to"]["task_id"] =
            target->getLabel();

        // FIXME: These have to be included into the port connection model!!!
        // doc["connections"][i]["transport"] = conn->getLabel();
        // doc["connections"][i]["type"] = "BUFFER";
        // doc["connections"][i]["size"] = 20;
    }
}

void CndModelWriter::update(const std::string& filename,
                            const BaseGraph::Ptr& graph)
{
    YAML::Node doc = YAML::LoadFile(filename);

    internal_write(doc, graph);

    std::ofstream fout;
    fout.open(filename.c_str());
    fout << doc;
    fout.close();
}

void CndModelWriter::write(const std::string& filename,
                           const BaseGraph::Ptr& graph)
{
    YAML::Node doc;

    internal_write(doc, graph);

    std::ofstream fout;
    fout.open(filename.c_str());
    fout << doc;
    fout.close();
}
}
}
