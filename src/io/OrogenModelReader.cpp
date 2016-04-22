#include "OrogenModelReader.hpp"

#include <graph_analysis/EdgeTypeManager.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

#include <graph_analysis/task_graph/DataType.hpp>
#include <graph_analysis/task_graph/DataValue.hpp>
#include <graph_analysis/task_graph/HasFeature.hpp>
#include <graph_analysis/task_graph/HasUniqueFeature.hpp>
#include <graph_analysis/task_graph/Property.hpp>
#include <graph_analysis/task_graph/Task.hpp>

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
typename T::Ptr createPort(const std::string& label, const std::string& type,
                           BaseGraph::Ptr graph, Vertex::Ptr parent)
{
    // Create port
    typename T::Ptr port = typename T::Ptr(new T(label));
    graph->addVertex(port);
    // Link port to parent
    task_graph::HasFeature::Ptr has = task_graph::HasFeature::Ptr(
        new task_graph::HasFeature(parent, port, "has"));
    graph->addEdge(has);
    // Create data type
    // TODO: Create a data type only if it is not yet known (?)
    // TODO: Create a data type modelling layer/view
    task_graph::DataType::Ptr dtype =
        task_graph::DataType::Ptr(new task_graph::DataType(type));
    graph->addVertex(dtype);
    // Link data type to port
    task_graph::HasUniqueFeature::Ptr hasU = task_graph::HasUniqueFeature::Ptr(
        new task_graph::HasUniqueFeature(port, dtype, "has-unique"));
    graph->addEdge(hasU);
    return port;
}

task_graph::Property::Ptr createProperty(const std::string& label,
                                         const std::string& type,
                                         const std::string& value,
                                         BaseGraph::Ptr graph,
                                         Vertex::Ptr parent)
{
    // Create property
    task_graph::Property::Ptr prop =
        task_graph::Property::Ptr(new task_graph::Property(label));
    graph->addVertex(prop);
    // Link property to parent
    task_graph::HasFeature::Ptr has = task_graph::HasFeature::Ptr(
        new task_graph::HasFeature(parent, prop, "has"));
    graph->addEdge(has);
    if (!type.empty())
    {
        // Create data type
        task_graph::DataType::Ptr dtype =
            task_graph::DataType::Ptr(new task_graph::DataType(type));
        graph->addVertex(dtype);
        // Link data type to property
        task_graph::HasUniqueFeature::Ptr hasU = task_graph::HasUniqueFeature::Ptr(
            new task_graph::HasUniqueFeature(prop, dtype, "has-unique"));
        graph->addEdge(hasU);
        if(!value.empty())
        {
            // Create data value
            task_graph::DataValue::Ptr dvalue =
                task_graph::DataValue::Ptr(new task_graph::DataValue(value));
            graph->addVertex(dvalue);
            // Link data value to property
            task_graph::HasUniqueFeature::Ptr hasU2 =
                task_graph::HasUniqueFeature::Ptr(
                    new task_graph::HasUniqueFeature(prop, dvalue, "has-unique"));
            graph->addEdge(hasU2);
        }
    }
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
        createPort<task_graph::InputPort>(current["Name"].as<std::string>(),
                                          current["Type"].as<std::string>(),
                                          graph, comp);
    }
    YAML::Node outputs = ports["outputPorts"]; // its a seq!
    for(it = outputs.begin(); it != outputs.end(); ++it)
    {
        YAML::Node current = *it;
        createPort<task_graph::OutputPort>(current["Name"].as<std::string>(),
                                           current["Type"].as<std::string>(),
                                           graph, comp);
    }

    // Add a 'state' property (compliance with CND model)
    createProperty("state",
                   "std::string",
                   "RUNNING",
                   graph,
                   comp);
    // Add a 'config_file' property (compliance with CND model)
    createProperty("config_file",
                   "std::string",
                   "",
                   graph,
                   comp);
    // Add a 'config_names' property (compliance with CND model)
    task_graph::Property::Ptr config =
    createProperty("config_names",
                   "",
                   "",
                   graph,
                   comp);
    createProperty("0",
                   "std::string",
                   "default",
                   graph,
                   config);

    // Finally properties of the task (if it has at least one)
    YAML::Node properties = ports["properties"];
    if (properties.begin() == properties.end())
        return;

    // First, we create a master property 'config' for the properties
    // (compliance with the CND model :/)
    config =
        task_graph::Property::Ptr(new task_graph::Property("config"));
    // Link property to component
    task_graph::HasFeature::Ptr has = task_graph::HasFeature::Ptr(
        new task_graph::HasFeature(comp, config, "has"));
    graph->addEdge(has);
    // Cycle through all and create
    for(it = properties.begin(); it != properties.end(); ++it)
    {
        YAML::Node current = *it;
        if(current["DefValue"])
            createProperty(current["Name"].as<std::string>(),
                           current["Type"].as<std::string>(),
                           current["DefValue"].as<std::string>(), graph,
                           config);
        else
            createProperty(current["Name"].as<std::string>(),
                           current["Type"].as<std::string>(), "", graph,
                           config);
    }
}

} // end namespace io
} // end namespace graph_analysis
