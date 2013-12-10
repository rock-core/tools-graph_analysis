#ifndef GRAPH_ANALYSIS_GRAPH_HPP
#define GRAPH_ANALYSIS_GRAPH_HPP

#include <graph_analysis/EdgeProperty.hpp>
#include <graph_analysis/VertexProperty.hpp>
#include <graph_analysis/Filter.hpp>

#include <boost/shared_ptr.hpp>

/**
 * The main namespace of this library
 */
namespace graph_analysis
{

/**
 * \brief General Graph template that should be implemented to wrap specific graph library
 * functionality
 * \tparam GraphType The underlying graph type of the wrapped library
 * \tparam VertexType The underlying vertex type of the wrapped library
 * \tparam EdgeType The underlying edge type of the wrapped library
 *
 */
template<typename GraphType, typename VertexType, typename EdgeType>
class BaseGraph
{
public:
    /**
     * \brief Default deconstructor
     */
    virtual ~BaseGraph() {}


    // Exporting the typenames so that they can be accesses
    // via BaseGraphType namespace
    typedef VertexType Vertex;
    typedef EdgeType Edge;
    typedef BaseGraph<GraphType,VertexType,EdgeType> BaseGraphType;
    typedef GraphType RawGraphType;

    /**
     * Type definitions that allow adding task properties
     */
    typedef graph_analysis::EdgeProperty<BaseGraphType, EdgeType> EdgePropertyType;
    typedef graph_analysis::VertexProperty<BaseGraphType, VertexType> VertexPropertyType;

    typedef typename EdgePropertyType::Ptr EdgePropertyTypePtr;
    typedef typename VertexPropertyType::Ptr VertexPropertyTypePtr;

    /**
     * \brief Add a vertex
     * \return the created vertex
     */
    virtual Vertex addVertex() = 0;

    /**
     * \brief Assign a vertex property
     */
    virtual void assignVertexProperty(const Vertex& v, VertexPropertyTypePtr property) = 0;

    /**
     * \brief Add an edge
     * \return the created edge
     */
    virtual Edge addEdge(const Vertex& u, const Vertex& v) = 0;

    /**
     * \brief Assign an edge property
     */
    virtual void assignEdgeProperty(const Edge& e, EdgePropertyTypePtr property) = 0;

    /**
     * \brief Get the source vertex for this edge
     * \return Pointer to the vertex data
     */
    virtual VertexPropertyTypePtr getSourceVertex(const Edge& e) const = 0;

    /**
     * \brief Get the target vertex for this edge
     * \return Pointer to the vertex data
     */
    virtual VertexPropertyTypePtr getTargetVertex(const Edge& e) const = 0;

    /**
     * \brief Get access to the underlying graph
     * \return the underlying graph instance
     */
    GraphType& raw() { return mGraph; }

protected:
    // The underlying graph instance
    GraphType mGraph;
};

/**
 * \brief BaseSubGraph represent a wrapper for libraries that use an invidual datatype for subgraphs
 * \details This wrapper guarantees that vertex filters and edgefilters won't be deallocated
 * as long as the subgraph is in use, e.g., for the lemon library the subgraph holds a reference to the main graph and only
 * associates filters -- but since the filters are added as references they need to be managed properly.
 * \tparam SubGraphType the underlying subgraph type
 * \tparam VertexFilter the vertex filter
 * \tparam EdgeFilter the edge filter
 */
template<typename SubGraphType, typename VertexFilter, typename EdgeFilter>
class BaseSubGraph
{
public:
    typedef SubGraphType GraphType;
    typedef BaseSubGraph<SubGraphType, VertexFilter, EdgeFilter> BaseSubGraphType;

    typedef VertexFilter VertexFilterType;
    typedef EdgeFilter EdgeFilterType;

    /**
     * This default Constructor will not(!) initialize the underlying subgraph type, but
     * only the given filters. 
     * Make sure that any class inheriting from this one properly initalizes the subgraph, i.e.
     * using setSubgraph
     *
     * Ownership of all objects, i.e. filters and graph is transferred to this class, which internally
     * uses shared pointer to guarantee cleanup when necessary
     */
    BaseSubGraph(VertexFilter* vertexFilter, EdgeFilter* edgeFilter)
    {
        mpVertexFilter = boost::shared_ptr<VertexFilter>(vertexFilter);
        mpEdgeFilter = boost::shared_ptr<EdgeFilter>(edgeFilter);
    }

    /**
     * Retrieve the raw subgraph type. Use with care since this object's memory might
     * be deallocated if you release your main object
     */
    SubGraphType& raw()
    {
        assert(mpSubgraph);
        return (*mpSubgraph);
    }

protected:
    /**
     * Get vertex filter
     * \return reference to vertex filter
     */
    VertexFilter& getVertexFilter() const
    {
        assert(mpVertexFilter);
        return *mpVertexFilter.get();
    }

    /**
     * Get edge filter
     * \return reference to edge filter
     */
    EdgeFilter& getEdgeFilter() const
    {
        assert(mpEdgeFilter);
        return *mpEdgeFilter.get();
    }

    /**
     * Set the subgraph
     * \param pointer to subgraph
     */
    void setSubgraph(SubGraphType* subgraph)
    {
        assert(subgraph);
        mpSubgraph = boost::shared_ptr<SubGraphType>(subgraph);
    }

    boost::shared_ptr<VertexFilter> mpVertexFilter;
    boost::shared_ptr<EdgeFilter> mpEdgeFilter;

    boost::shared_ptr<SubGraphType> mpSubgraph;
};


}
#endif // GRAPH_ANALYSIS_GRAPH_HPP
