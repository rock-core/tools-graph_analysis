#include "TaskTemplate.hpp"

#include <graph_analysis/io/OrogenModelReader.hpp>

namespace graph_analysis
{
namespace task_graph
{
    TaskTemplate::TaskTemplate(const std::string &yamlFileName) {
        // Create a new BaseGraph
        mpBaseGraph = BaseGraph::getInstance();
        // Fill the BaseGraph from yaml file
        io::OrogenModelReader reader;
        reader.read(yamlFileName, mpBaseGraph);
        // Find root (task) vertex
        std::vector<Vertex::Ptr> vertices = mpBaseGraph->getAllVertices();
        std::vector<Vertex::Ptr>::const_iterator it = vertices.begin();
        for (;it != vertices.end(); ++it)
        {
            if ((*it)->getClassName() == Task::vertexType())
                break;
        }
        mpRootVertex = dynamic_pointer_cast<Task>(*it);
        // Store the yaml file name
        mYamlFileName = yamlFileName;
    }

    BaseGraph::Ptr TaskTemplate::instantiateTask() {
        return (mpBaseGraph->clone());
    }

    void TaskTemplate::instantiateAndAddTask(BaseGraph::Ptr graph) {
        // Create a copy of the template graph
        BaseGraph::Ptr copy = instantiateTask();
        // Add all vertices and edges to the target graph
        VertexIterator::Ptr vit = copy->getVertexIterator();
        while (vit->next())
            graph->addVertex(vit->current());
        EdgeIterator::Ptr eit = copy->getEdgeIterator();
        while (eit->next())
            graph->addEdge(eit->current());
        // Intentionally release the shared ptr
        copy.reset();
    }
}
}
