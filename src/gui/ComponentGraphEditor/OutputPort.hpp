#ifndef GRAPH_ANALYSIS_OUTPUTPORT_HPP
#define GRAPH_ANALYSIS_OUTPUTPORT_HPP

#include "Port.hpp"

namespace graph_analysis {
namespace gui {

/**
 * \brief An OutputPort vertex inherited to allow storing data of output Port type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Port
 */
class OutputPort : public Port
{
public:
    OutputPort(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::OutputPort"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::OutputPort"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual OutputPort* getClone() const { return new OutputPort(*this); }

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_OUTPUTPORT_HPP
