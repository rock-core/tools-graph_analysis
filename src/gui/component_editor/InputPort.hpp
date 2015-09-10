#ifndef GRAPH_ANALYSIS_INPUTPORT_HPP
#define GRAPH_ANALYSIS_INPUTPORT_HPP

#include <graph_analysis/gui/component_editor/Port.hpp>

namespace graph_analysis {
namespace gui {

/**
 * \brief An InputPort vertex inherited to allow storing data of input Port type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Port
 */
class InputPort : public Port
{
public:
    InputPort(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::InputPort"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::InputPort"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual InputPort* getClone() const { return new InputPort(*this); }

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_INPUTPORT_HPP
