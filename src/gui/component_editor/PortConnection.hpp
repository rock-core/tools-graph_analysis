#ifndef GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_PORT_CONNECTION_HPP
#define GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_PORT_CONNECTION_HPP

#include <graph_analysis/Edge.hpp>
#include <graph_analysis/gui/component_editor/InputPort.hpp>
#include <graph_analysis/gui/component_editor/OutputPort.hpp>

namespace graph_analysis {
namespace gui {

class PortConnection : public Edge
{
public:
    typedef boost::shared_ptr<PortConnection> Ptr;

    PortConnection(const OutputPort::Ptr& output, const InputPort::Ptr& input, const std::string& label)
        : Edge(output, input, label)
    {}

    PortConnection(const std::string& label = "")
        : Edge(label)
    {}

    virtual std::string getClassName() const { return edgeType(); }

    static std::string edgeType() { return "PortConnection"; }
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_PORT_CONNECTION_HPP
