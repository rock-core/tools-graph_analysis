#ifndef GRAPH_ANALYSIS_TASK_GRAPH_INSTANCE_OF_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_INSTANCE_OF_HPP

#include <graph_analysis/Edge.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis
{
namespace task_graph
{

class InstanceOf : public Edge
{
public:
    typedef shared_ptr<InstanceOf> Ptr;

    InstanceOf(const Vertex::Ptr& base, const Vertex::Ptr& instantiated,
               const std::string& label)
        : Edge(base, instantiated, label)
    {
        // TODO: Check that instantiated has one and only one InstanceOf
        // relation!!!
    }

    InstanceOf(const std::string& label = "")
        : Edge(label)
    {
    }

    virtual std::string getClassName() const
    {
        return edgeType();
    }

    static std::string edgeType()
    {
        return "graph_analysis::task_graph::InstanceOf";
    }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual InstanceOf* getClone() const
    {
        return new InstanceOf(*this);
    }
};
}
}
#endif
