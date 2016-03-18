#ifndef GRAPH_ANALYSIS_COMPONENT_HPP
#define GRAPH_ANALYSIS_COMPONENT_HPP

#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/Graph.hpp>

#include <graph_analysis/gui/ComponentGraphEditor/OutputPort.hpp>
#include <graph_analysis/gui/ComponentGraphEditor/InputPort.hpp>

namespace graph_analysis
{

/**
 * \brief A Cluster vertex inherited to allow storing data of Cluster type
 * \details For the purpose of storing data with a vertex, this storage type
 * inherits
 * from class Vertex
 */
class Component : public Vertex
{
  public:
    typedef shared_ptr< Component > Ptr;

    Component(const std::string &label = "");
    /** Get class name
     * \return class name
     */
    std::string getClassName() const    { return Component::vertexType(); }

    static std::string vertexType() { return "graph_analysis::Component"; }

    std::vector<OutputPort::Ptr> getOutputPorts(const BaseGraph::Ptr &graph) const;
    std::vector<InputPort::Ptr> getInputPorts(const BaseGraph::Ptr &graph) const;

  protected:
    /**
     * Create a copy of this vertex
     */
    virtual Component *getClone() const { return new Component(*this); }
};

} // end namespace graph_analysis
#endif
