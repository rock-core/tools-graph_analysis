#ifndef GRAPH_ANALYSIS_TASK_GRAPH_PORT_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_PORT_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/BaseGraph.hpp>

namespace graph_analysis
{
namespace task_graph {

class Task;

/**
 * \brief A Port vertex inherited to allow storing data of Port type
 * \details For the purpose of storing data with a vertex, this storage type
 * inherits
 * from class Vertex
 */
class Port : public Vertex
{
public:
    typedef shared_ptr<Port> Ptr;

    Port(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const
    {
        return vertexType();
    }

    /** Get class type
     * \return class type
     */
    static std::string vertexType()
    {
        return "graph_analysis::task_graph::Port";
    }

    /**
     * @brief returns the task this port belongs to
     *
     * will print an error if there is no task
     *
     * not so nice: forward declaration of shared pointer...
     *
     * @param graph the graph to work on
     * @return shared pointer of the relevant task. will be null if there
     *         is no task
     */
    shared_ptr<Task> getTask(const BaseGraph::Ptr& graph) const;

    /**
     * @brief counts the number of "PortConnection" edges attached to this
     *vertex
     *
     * prints an error message if more than one PortConnection is detected.
     *
     * @param graph the graph to work on
     * @return true if there is one or more PortConnection attached
     */
    bool isConnected(const BaseGraph::Ptr& graph) const;

protected:
    /**
     * Create a copy of this vertex
     */
    virtual Port* getClone() const
    {
        return new Port(*this);
    }
};

}
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PORT_HPP
