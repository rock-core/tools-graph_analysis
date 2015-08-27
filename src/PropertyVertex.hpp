#ifndef GRAPH_ANALYSIS_PROPERTYVERTEX_HPP
#define GRAPH_ANALYSIS_PROPERTYVERTEX_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>
#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis
{

/**
 * \brief A Property vertex inherited to allow storing data of Property type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class PropertyVertex : public Vertex
{
public:
    PropertyVertex(const std::string& label = std::string());
    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::PropertyVertex"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::PropertyVertex"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual PropertyVertex* getClone() const { return new PropertyVertex(*this); }

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_PROPERTYVERTEX_HPP
