#ifndef GRAPH_ANALYSIS_TASK_GRAPH_TASK_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_TASK_HPP

// TODO: Add PropertyPort class and functions

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Graph.hpp>

#include "OutputPort.hpp"
#include "InputPort.hpp"

namespace graph_analysis
{
namespace task_graph {

/**
 * \brief A Cluster vertex inherited to allow storing data of Cluster type
 * \details For the purpose of storing data with a vertex, this storage type
 * inherits
 * from class Vertex
 */
class Task : public Vertex
{
  public:
    typedef shared_ptr< Task > Ptr;

    Task(const std::string &label = "");
    Task(const std::string &templateLabel, const std::string &label = "");

    /** Get class name
     * \return class name
     */
    std::string getClassName() const    { return Task::vertexType(); }

    static std::string vertexType() { return "graph_analysis::task_graph::Task"; }

    std::vector<OutputPort::Ptr> getOutputPorts(const BaseGraph::Ptr &graph) const;
    std::vector<InputPort::Ptr> getInputPorts(const BaseGraph::Ptr &graph) const;

    std::string getTemplateLabel() const;
    void setTemplateLabel (const std::string& templateLabel);

  protected:
    /**
     * Create a copy of this vertex
     */
    virtual Task *getClone() const { return new Task(*this); }

  private:
    /**
     * Specifies the template from which this task has been instantiated
     */
    std::string mTemplateLabel;
};

}
} // end namespace graph_analysis
#endif
