#ifndef GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_HAS_FEATURE_HPP
#define GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_HAS_FEATURE_HPP

#include <graph_analysis/Edge.hpp>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {

class HasFeature : public Edge
{
public:
    typedef shared_ptr<HasFeature> Ptr;

    HasFeature(const Vertex::Ptr& output, const Vertex::Ptr& input, const std::string& label)
        : Edge(output, input, label)
    {}

    HasFeature(const std::string& label = "")
        : Edge(label)
    {}

    virtual std::string getClassName() const { return edgeType(); }

    static std::string edgeType() { return "graph_analysis::HasFeature"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual HasFeature* getClone() const { return new HasFeature(*this); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_PORT_CONNECTION_HPP
