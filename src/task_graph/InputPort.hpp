#ifndef GRAPH_ANALYSIS_TASK_GRAPH_INPUTPORT_HPP
#define GRAPH_ANALYSIS_TASK_GRAPH_INPUTPORT_HPP

#include "Port.hpp"

namespace graph_analysis {
namespace task_graph {

/**
 * \brief An InputPort vertex inherited to allow storing data of input Port type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Port
 */
class InputPort : public Port
{
public:
    typedef shared_ptr< InputPort > Ptr;
    InputPort(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return vertexType(); }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::task_graph::InputPort"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual InputPort* getClone() const { return new InputPort(*this); }

};

}
} // end namespace graph_analysis
#endif
