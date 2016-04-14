#ifndef GRAPH_ANALYSIS_TASK_GRAPH_TASK_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_TASK_HPP

// TODO: Add PropertyPort class and functions

#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Vertex.hpp>

#include "InputPort.hpp"
#include "OutputPort.hpp"

namespace graph_analysis
{
namespace task_graph
{

class TaskTemplate;
typedef shared_ptr<TaskTemplate> TaskTemplatePtr;

/**
 * \brief A Cluster vertex inherited to allow storing data of Cluster type
 * \details For the purpose of storing data with a vertex, this storage type
 * inherits
 * from class Vertex
 */
class Task : public Vertex
{
public:
    typedef shared_ptr<Task> Ptr;

    Task(const std::string& label = "");

    /** Get class name
     * \return class name
     */
    std::string getClassName() const
    {
        return Task::vertexType();
    }

    static std::string vertexType()
    {
        return "graph_analysis::task_graph::Task";
    }

    /**
     * \brief Search a graph for vertices of class OutputPort which are
     * associated to us via HasFeature edge
     */
    std::vector<OutputPort::Ptr>
    getOutputPorts(const BaseGraph::Ptr& graph) const;
    std::vector<InputPort::Ptr>
    getInputPorts(const BaseGraph::Ptr& graph) const;

    /**
     * \brief Search a graph for a TaskTemplate of which this task is an
     * INSTANCE-OF
     */
    TaskTemplatePtr getTemplate(const BaseGraph::Ptr& graph) const;

protected:
    /**
     * Create a copy of this vertex
     */
    virtual Task* getClone() const
    {
        return new Task(*this);
    }
};
}
} // end namespace graph_analysis
#endif
