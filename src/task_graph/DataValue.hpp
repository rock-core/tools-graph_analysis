#ifndef GRAPH_ANALYSIS_TASK_GRAPH_DATAVALUE_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_DATAVALUE_HPP

#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis
{
namespace task_graph
{

/**
 * \brief A DataValue vertex which specifies the data type of a property/port
 * \details For the purpose of storing data with a vertex, this storage type
 * inherits
 * from class Vertex
 */
class DataValue : public Vertex
{
public:
    typedef shared_ptr<DataValue> Ptr;

    DataValue(const std::string& label = "");

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
        return "graph_analysis::task_graph::DataValue";
    }

    /**
     * @brief returns the task this port belongs to
     *
     * @param graph the graph to work on
     * @return shared pointer to the asssociated vertex
     */
    Vertex::Ptr getOwner(const BaseGraph::Ptr& graph) const;

protected:
    /**
     * Create a copy of this vertex
     */
    virtual DataValue* getClone() const
    {
        return new DataValue(*this);
    }
};
}
}
#endif
