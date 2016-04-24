#ifndef GRAPH_ANALYSIS_TASK_GRAPH_TASK_TEMPLATE_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_TASK_TEMPLATE_HPP

#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis
{
class BaseGraph;

namespace task_graph
{
class Task;
typedef shared_ptr<Task> TaskPtr;

class TaskTemplate : public Vertex
{
public:
    typedef shared_ptr<TaskTemplate> Ptr;

    /**
     * The default constructor creates an invalid task template !!!
     */
    TaskTemplate(const std::string& label = "");

    /** Get class name
     * \return class name
     */
    std::string getClassName() const
    {
        return TaskTemplate::vertexType();
    }

    static std::string vertexType()
    {
        return "graph_analysis::task_graph::TaskTemplate";
    }

    /**
     * \brief Creates a usable TaskTemplate from a yaml file spec
     */
    void loadFromFile(const std::string& yamlFileName);

    /**
     * If called, creates a new instance and adds it to the given graph argument
     * The root vertex of the new instance is returned.
     */
    TaskPtr instantiateAndAddTo(BaseGraph::Ptr graph,
                                const std::string& label = "");

    std::string const& yamlFileName() const
    {
        return mYamlFileName;
    }

    TaskPtr const rootVertex() const
    {
        return mpRootVertex;
    }

    const BaseGraph::Ptr& getInternalBaseGraph() const;

protected:
    /**
     * Create a copy of this vertex
     */
    virtual TaskTemplate* getClone() const
    {
        return new TaskTemplate(*this);
    }

private:
    std::string mYamlFileName;

    int mInstance;
    BaseGraph::Ptr mpBaseGraph;
    TaskPtr mpRootVertex;
};
}
}
#endif
