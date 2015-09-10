#ifndef GRAPH_ANALYSIS_CLUSTER_HPP
#define GRAPH_ANALYSIS_CLUSTER_HPP

#include <iostream>
#include <graph_analysis/Vertex.hpp>

namespace graph_analysis {
namespace gui {

/**
 * \brief A Cluster vertex inherited to allow storing data of Cluster type
 * \details For the purpose of storing data with a vertex, this storage type inherits
 * from class Vertex
 */
class Cluster : public Vertex
{
public:
    Cluster(const std::string& label = "");
    /** Get class name
     * \return class name
     */
    std::string getClassName() const    { return Cluster::vertexType(); }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::Cluster"; }

protected:
    /**
     * Create a copy of this vertex
     */
    virtual Cluster* getClone() const { return new Cluster(*this); }

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_CLUSTER_HPP
