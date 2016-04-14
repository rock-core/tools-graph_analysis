#include "CndModelWriter.hpp"

#include <fstream>
#include <graph_analysis/task_graph/HasFeature.hpp>
#include <graph_analysis/task_graph/PortConnection.hpp>
#include <graph_analysis/task_graph/Task.hpp>
#include <graph_analysis/task_graph/TaskTemplate.hpp>
#include <iostream>
#include <yaml-cpp/yaml.h>

namespace graph_analysis
{
namespace io
{

std::string createRubyStuff(const std::string& templateLabel)
{
    std::string prefix = "orogen_default_";
    std::string::size_type pos = templateLabel.find("::");
    std::string base = templateLabel.substr(0, pos);
    std::string name = templateLabel.substr(pos + 2);

    return (prefix + base + "__" + name);
}

void CndModelWriter::write(const std::string& filename,
                           const BaseGraph::Ptr& graph)
{
    YAML::Node doc;
    int i = 1;

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

        // FIXME: These have to be included into the task model!!!
        doc["tasks"][task->getLabel()]["state"] = "RUNNING";
        // doc["tasks"][task->getLabel()]["config_file"] = "";
        doc["tasks"][task->getLabel()]["config_names"][0] = "default";
        // doc["tasks"][task->getLabel()]["config"] = "";

        // FIXME: We dont know the concept of deployments yet, so we create 1
        // deployment/task
        // NOTE: This is a default deployment
        doc["deployments"][i]["deployer"] = "orogen";
        doc["deployments"][i]["hostID"] = "localhorst";
        std::string rubyShit = createRubyStuff(taskTemp->getLabel());
        doc["deployments"][i]["process_name"] = rubyShit;
        doc["deployments"][i]["taskList"][task->getLabel()] = rubyShit;
        doc["deployments"][i]["taskList"][task->getLabel() + "_Logger"] =
            rubyShit + "_Logger";
        i++;
    }

    // Cycle thorugh all edges
    i = 1;
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
        doc["connections"][i]["from"]["port_name"] = output->getLabel();
        doc["connections"][i]["to"]["port_name"] = input->getLabel();

        // Find the corresponding source and target task vertices
        task_graph::Task::Ptr source = output->getTask(graph);
        task_graph::Task::Ptr target = input->getTask(graph);
        doc["connections"][i]["from"]["task_id"] = source->getLabel();
        doc["connections"][i]["to"]["task_id"] = target->getLabel();
        doc["connections"][i]["transport"] = conn->getLabel();

        // FIXME: These have to be included into the port connection model!!!
        doc["connections"][i]["type"] = "BUFFER";
        doc["connections"][i]["size"] = 20;
        i++;
    }

    std::ofstream fout;
    fout.open(filename.c_str());
    fout << doc;
    fout.close();
}
}
}
