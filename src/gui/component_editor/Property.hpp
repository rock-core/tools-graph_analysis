#ifndef GRAPH_ANALYSIS_PROPERTY_HPP
#define GRAPH_ANALYSIS_PROPERTY_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace gui {

/**
 * \brief A Property vertex inherited to allow storing data of Property type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class Property : public Vertex
{
public:
    Property(const std::string& label = std::string());
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::Property"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::Property"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual Property* getClone() const { return new Property(*this); }

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PROPERTY_HPP
