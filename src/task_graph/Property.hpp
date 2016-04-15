#ifndef GRAPH_ANALYSIS_TASK_GRAPH_PROPERTY_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_PROPERTY_HPP

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/BaseGraph.hpp>

namespace graph_analysis
{
namespace task_graph {

/**
 * \brief A Property vertex which specifies a named property of another vertex
 * \details For the purpose of storing data with a vertex, this storage type
 * inherits
 * from class Vertex
 */
class Property : public Vertex
{
public:
    typedef shared_ptr<Property> Ptr;

    Property(const std::string& label = "");

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
        return "graph_analysis::task_graph::Property";
    }

    /**
     * @brief returns the task this port belongs to
     *
     * @param graph the graph to work on
     * @return shared pointer to the asssociated vertex
     */
    Vertex::Ptr getOwner(const BaseGraph::Ptr& graph) const;

    /**
     * \brief A string defining the type of data this property holds
     */
    std::string mType;
    std::string mValue;

protected:
    /**
     * Create a copy of this vertex
     */
    virtual Property* getClone() const
    {
        return new Property(*this);
    }
};

}
}
#endif

