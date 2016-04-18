#ifndef GRAPH_ANALYSIS_TASK_GRAPH_DATATYPE_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_DATATYPE_HPP

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/BaseGraph.hpp>

namespace graph_analysis
{
namespace task_graph {

/**
 * \brief A DataType vertex which specifies the data type of a property/port
 * \details For the purpose of storing data with a vertex, this storage type
 * inherits
 * from class Vertex
 */
class DataType : public Vertex
{
public:
    typedef shared_ptr<DataType> Ptr;

    DataType(const std::string& label = "");

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
        return "graph_analysis::task_graph::DataType";
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
    virtual DataType* getClone() const
    {
        return new DataType(*this);
    }
};

}
}
#endif

