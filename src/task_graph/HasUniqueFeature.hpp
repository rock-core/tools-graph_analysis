#ifndef GRAPH_ANALYSIS_TASK_GRAPH_HAS_UNIQUE_FEATURE_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_HAS_UNIQUE_FEATURE_HPP

#include <graph_analysis/Edge.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace task_graph {

class HasUniqueFeature : public Edge
{
public:
    typedef shared_ptr<HasUniqueFeature> Ptr;

    HasUniqueFeature(const Vertex::Ptr& parent, const Vertex::Ptr& child, const std::string& label)
        : Edge(parent, child, label)
    {
        // TODO: Check that parent has no other child with the same type and the same label!!!
    }

    HasUniqueFeature(const std::string& label = "")
        : Edge(label)
    {}

    virtual std::string getClassName() const { return edgeType(); }

    static std::string edgeType() { return "graph_analysis::task_graph::HasUniqueFeature"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual HasUniqueFeature* getClone() const { return new HasUniqueFeature(*this); }
};

}
}
#endif

