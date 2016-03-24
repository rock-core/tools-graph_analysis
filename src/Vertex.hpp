#ifndef GRAPH_ANALYSIS_VERTEX_HPP
#define GRAPH_ANALYSIS_VERTEX_HPP

#include <graph_analysis/GraphElement.hpp>

namespace graph_analysis
{

class BaseGraph;

/**
 * \brief A vertex inherited to allow storing data of any type
 * \details For the purpose of storing data with a vertex, the storage type needs to inherit
 * from this class
 */
class Vertex : public GraphElement
{
public:
    typedef shared_ptr< Vertex > Ptr;

    virtual ~Vertex() {}

    Vertex(const std::string& label = std::string());

    /**
     * Clone this vertex -- the returned vertex will have no
     * graph association
     */
    Vertex::Ptr clone() const;

    /** Get class name
     * \return class name
     */
    virtual std::string getClassName() const { return "graph_analysis::Vertex"; }

    /** Get class type
     * \return class type
     */
    static std::string vertexType() { return "graph_analysis::Vertex"; }

    /**
     * Convert element to string
     */
    virtual std::string toString() const;

    /**
     * Convert element to string and apply a given indentation (in number of
     * spaces)
     */
    virtual std::string toString(uint32_t indentation) const;

    /**
     * in order for child-classes to iterate the graph, a way to obtain the
     * shared pointer actually respresenting this vertex on a particular graph is
     * needed.
     *
     * \param pGraph shared pointer to the graph where this vertex is contained
     * \return shared pointer of this vertex belonging to the given graph
     */
    Vertex::Ptr getSharedPointer(const shared_ptr<BaseGraph>& pGraph) const;

protected:
    /**
     * Create a copy of this vertex using a explicit clone of 
     * the subclass of Vertex
     \verbatim
     return new MyVertex(*this);
     \endverbatim
     */
    virtual Vertex* getClone() const { return new Vertex(*this); }

};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_VERTEX_HPP
