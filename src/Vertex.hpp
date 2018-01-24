#ifndef GRAPH_ANALYSIS_VERTEX_HPP
#define GRAPH_ANALYSIS_VERTEX_HPP

#include <vector>
#include <set>
#include "GraphElement.hpp"

namespace graph_analysis
{

class BaseGraph;
class VertexTypeManager;

/**
 * \brief A vertex inherited to allow storing data of any type
 * \details For the purpose of storing data with a vertex, the storage type needs to inherit
 * from this class
 */
class Vertex : public GraphElement
{
public:
    typedef shared_ptr< Vertex > Ptr;
    typedef std::vector< Ptr > PtrList;
    typedef std::set< Ptr > PtrSet;

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
    Vertex::Ptr getSharedPointerFromGraph(const shared_ptr<BaseGraph>& pGraph) const;

    /**
     * Hook to register attributes of this vertex, in order to properly
     * perform serialization
     *
     * If serialization is not needed, then registration is optional
     * Otherwise each Vertex implementation has to implement this
     * method, and add a static member
     \verbatim
     #include "VertexRegistration.hpp"

     class DerivedVertex : Vertex
     {
        static const VertexRegistration<DerivedVertex> msRegistration;
     }
     \endverbatim
     * in the source file, e.g. DerivedVertex.cpp add
     \verbatim
     const VertexRegistration<DerivedVertex> DerivedVertex::msRegistration;
     \endverbatim
     */
    virtual void registerAttributes(VertexTypeManager*) const {}

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
