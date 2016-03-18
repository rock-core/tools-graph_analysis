#ifndef GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_PORT_CONNECTION_HPP
#define GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_PORT_CONNECTION_HPP

#include <graph_analysis/Edge.hpp>
#include "InputPort.hpp"
#include "OutputPort.hpp"

namespace graph_analysis {

class PortConnection : public Edge
{
public:
    typedef shared_ptr<PortConnection> Ptr;

    PortConnection(const OutputPort::Ptr& output, const InputPort::Ptr& input, const std::string& label)
        : Edge(output, input, label)
    {}

    PortConnection(const std::string& label = "")
        : Edge(label)
    {}

    virtual std::string getClassName() const { return edgeType(); }

    static std::string edgeType() { return "graph_analysis::PortConnection"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual PortConnection* getClone() const { return new PortConnection(*this); }
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_COMPONENT_EDITOR_PORT_CONNECTION_HPP
