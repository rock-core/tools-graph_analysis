#ifndef GRAPH_ANALYSIS_EDGE_PROPERTY_HPP
#define GRAPH_ANALYSIS_EDGE_PROPERTY_HPP

#include <boost/shared_ptr.hpp>

namespace graph_analysis {

template<typename EdgeType>
class EdgeProperty
{
public:
    typedef boost::shared_ptr< EdgeProperty<EdgeType> > Ptr;

    // Get class name
    // \return class name
    virtual std::string getClassName() const { return "graph_analysis::EdgeProperty"; }

    /**
     * Set the corresponding edge
     */
    void setEdge(const EdgeType& edge) { mEdge = edge; }

    /**
     * Get edge
     */
    EdgeType getEdge() { return mEdge; }

private:
    EdgeType mEdge;

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_EDGE_PROPERTY_HPP
