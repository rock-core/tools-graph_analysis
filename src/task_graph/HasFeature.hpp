#ifndef GRAPH_ANALYSIS_TASK_GRAPH_HAS_FEATURE_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_HAS_FEATURE_HPP

#include <graph_analysis/Edge.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace task_graph {

class HasFeature : public Edge
{
public:
    typedef shared_ptr<HasFeature> Ptr;

    HasFeature(const Vertex::Ptr& parent, const Vertex::Ptr& child, const std::string& label)
        : Edge(parent, child, label)
    {
        // TODO: Check that parent has no other child of the same type AND the same label
    }

    HasFeature(const std::string& label = "")
        : Edge(label)
    {}

    virtual std::string getClassName() const { return edgeType(); }

    static std::string edgeType() { return "graph_analysis::task_graph::HasFeature"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual HasFeature* getClone() const { return new HasFeature(*this); }
};

}
}
#endif
