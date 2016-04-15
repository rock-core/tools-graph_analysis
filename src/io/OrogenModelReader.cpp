#include "OrogenModelReader.hpp"

#include <graph_analysis/EdgeTypeManager.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

#include <graph_analysis/task_graph/HasFeature.hpp>
#include <graph_analysis/task_graph/HasUniqueFeature.hpp>
#include <graph_analysis/task_graph/Task.hpp>
#include <graph_analysis/task_graph/Property.hpp>

// Include some yaml parsing stuff
#include <yaml-cpp/yaml.h>

namespace graph_analysis
{
namespace io
{

// switch (inputs.Type()) {
// case YAML::NodeType::Null:
// std::cout << "NULL";
// break;
// case YAML::NodeType::Scalar:
// std::cout << "SCALAR";
// break;
// case YAML::NodeType::Sequence:
// std::cout << "SEQ";
// break;
// case YAML::NodeType::Map:
// std::cout << "MAP";
// break;
// case YAML::NodeType::Undefined:
// std::cout << "UNDEF";
// break;
// default:
// std::cout << "W00t?!";
// break;
//}
// std::cout << std::endl;

template <typename T>
typename T::Ptr createPort(const std::string& label, const std::string& type, BaseGraph::Ptr graph,
                Vertex::Ptr parent)
{
    typename T::Ptr port = typename T::Ptr(new T(label));
    port->mType = type;
    graph->addVertex(port);
    task_graph::HasFeature::Ptr has = task_graph::HasFeature::Ptr(
        new task_graph::HasFeature(parent, port, "has"));
    graph->addEdge(has);
    return port;
}

task_graph::Property::Ptr createProperty(const std::string& label, const std::string& type, const std::string& value, BaseGraph::Ptr graph,
                Vertex::Ptr parent)
{
    task_graph::Property::Ptr prop = task_graph::Property::Ptr(new task_graph::Property(label));
    prop->mType = type;
    prop->mValue = value;
    graph->addVertex(prop);
    task_graph::HasUniqueFeature::Ptr has = task_graph::HasUniqueFeature::Ptr(
            new task_graph::HasUniqueFeature(parent, prop, "has-unique"));
    graph->addEdge(has);
    return prop;
}

void OrogenModelReader::read(const std::string& filename, BaseGraph::Ptr graph)
{
    // handy pointers
    // VertexTypeManager *vManager = VertexTypeManager::getInstance();
    // EdgeTypeManager *eManager = EdgeTypeManager::getInstance();

    // remove previous graph
    graph->clear();

    // Load and parse the yaml file
    YAML::Node task = YAML::LoadFile(filename);
    YAML::const_iterator it = task.begin();
    std::string baseClassName = it->first.as<std::string>();
    it = it->second.begin();
    std::string className = it->first.as<std::string>();
    // At first create the master node representing the whole task
    task_graph::Task::Ptr comp = task_graph::Task::Ptr(
        new task_graph::Task(baseClassName + "::" + className));
    graph->addVertex(comp);

    // Then we create all input ports and relate them to the master node
    YAML::Node ports = it->second;           // its a map!
    YAML::Node inputs = ports["inputPorts"]; // its a seq!
    for(it = inputs.begin(); it != inputs.end(); ++it)
    {
        YAML::Node current = *it;
        createPort<task_graph::InputPort>(current["Name"].as<std::string>(), current["Type"].as<std::string>(),
                                          graph, comp);
    }
    YAML::Node outputs = ports["outputPorts"]; // its a seq!
    for(it = outputs.begin(); it != outputs.end(); ++it)
    {
        YAML::Node current = *it;
        createPort<task_graph::OutputPort>(current["Name"].as<std::string>(), current["Type"].as<std::string>(),
                                           graph, comp);
    }
    // Finally properties of the task
    YAML::Node properties = ports["properties"];
    for(it = properties.begin(); it != properties.end(); ++it)
    {
        YAML::Node current = *it;
        if (current["DefValue"])
            createProperty(current["Name"].as<std::string>(), current["Type"].as<std::string>(), current["DefValue"].as<std::string>(),
                                               graph, comp);
        else
            createProperty(current["Name"].as<std::string>(), current["Type"].as<std::string>(), "",
                                               graph, comp);
    }
}

} // end namespace io
} // end namespace graph_analysis
