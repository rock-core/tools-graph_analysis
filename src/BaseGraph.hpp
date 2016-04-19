#ifndef GRAPH_ANALYSIS_BASE_GRAPH_HPP
#define GRAPH_ANALYSIS_BASE_GRAPH_HPP

#include <graph_analysis/SharedPtr.hpp>
#include <graph_analysis/EdgeIterator.hpp>
#include <graph_analysis/VertexIterator.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/SubGraph.hpp>
#include <graph_analysis/Algorithms.hpp>
#include <graph_analysis/BaseIterable.hpp>
#include <graph_analysis/BaseGraphObserver.hpp>

#include <set>

/**
 * The main namespace of this library
 */
namespace graph_analysis
{

/**
 * \brief General Graph template that should be implemented to wrap specific graph library
 * functionality
 * We assume that all graph implementations will rely and allow access to vertices and edges via
 * integers
 */
class BaseGraph : public VertexIterable, public EdgeIterable, public Algorithms
{

public:
    enum ImplementationType { BOOST_DIRECTED_GRAPH, LEMON_DIRECTED_GRAPH, SNAP_DIRECTED_GRAPH, IMPLEMENTATION_TYPE_END };
    static std::map<ImplementationType, std::string> ImplementationTypeTxt;

    typedef shared_ptr<BaseGraph> Ptr;

    /**
     * \brief Default deconstructor
     */
    virtual ~BaseGraph() {}

    /**
     * Get a graph instance of the given implementation type
     * \return Ptr to new instance
     */
    static Ptr getInstance(ImplementationType type = LEMON_DIRECTED_GRAPH);

    /**
     * Add an observer to the set of observers (see BaseGraphObserver.hpp)
     */
    void addObserver(const BaseGraphObserver::Ptr& observer);

    /**
     * Removes an observer from the set of observers.
     */
    void removeObserver(const BaseGraphObserver::Ptr& observer);

    /**
     * Create a copy of this graph
     * The copy of the graph and this graph will still share the same set of
     * edges and vertices.
     * If you require complete separation consider calling clone
     * \return pointer to the copy of this graph
     */
    virtual BaseGraph::Ptr copy() const { throw std::runtime_error("BaseGraph::copy: not implemented"); }

    /**
     * Clone the graph, i.e. provides a deep copy of this graph so that
     * this graph and the clone do not share any references
     */
    BaseGraph::Ptr clone() const;

    /**
     * Copy the vertices but clone the edges of the graph
     */
    BaseGraph::Ptr cloneEdges() const;

    /**
     * Allow to create an instance of the same type of graph
     */
    virtual BaseGraph::Ptr newInstance() const { throw std::runtime_error("BaseGraph::newInstance: not implemented"); }


    /**
     * Get the subgraph for a graph.
     * To support this feature implementation have to provide
     * an implementation of BaseGraph::createSubGraph
     * \see createSubGraph
     */
    static SubGraph::Ptr getSubGraph(const Ptr& graph);

    /**
     * Get the implementation type of this graph object
     * \return implementation type
     */
    ImplementationType getImplementationType() const { return mImplementationType; }

    /**
     * Get the implementation type as string
     * \return Stringified implementation type
     */
    std::string getImplementationTypeName() const { return ImplementationTypeTxt[mImplementationType]; }

    /**
     * \brief tests whether the graph is void/empty
     * \return true when the graph is empty (does not contain a single vertex); false otherwise
     */
    bool empty() const;

    /**
     * \brief Add a vertex
     * \details
     * The base implementation does not add the vertex to the graph, but checks only if the
     * vertex is already associated with the graph.
     * Reimplement for the specific graph implementation and call the base function first, i.e.
     * BaseGraph::addVertex(v)
     * \throws std::runtime_error if the vertex already exists in the graph
     * \returns GraphElementId which identifies the vertex in this(!) graph
     * instance
     */
    GraphElementId addVertex(const Vertex::Ptr& vertex);

    /**
     * \brief Get the vertex id for this graph
     * \throw std::runtime_error if the vertex is not part of this graph
     */
    GraphElementId getVertexId(const Vertex::Ptr& vertex) const { (void) vertex; return dynamic_pointer_cast<GraphElement>(vertex)->getId( getId() ); }

    /**
     * \brief Get the vertex by id
     * \return vertex
     */
    virtual Vertex::Ptr getVertex(GraphElementId id) const { (void) id; throw std::runtime_error("graph_analysis::BaseGraph::getVertex has not been implemented"); }

    /**
     * \brief Remove vertex
     * In order to reimplement, call the base function first
     * BaseGraph::addVertex(v)
     */
    void removeVertex(const Vertex::Ptr& vertex);

    /**
     * \brief Add an edge
     * Cannot be reimplemented directly, but see addEdge(Edge::Ptr, GraphElementId, GraphElementId)
     */
    GraphElementId addEdge(const Edge::Ptr& edge);

    /**
     * Remove an edge and disassociate from this graph
     * In order to reimplement, call the base function first
     * BaseGraph::addVertex(v)
     */
    void removeEdge(const Edge::Ptr& edge);

    /**
     * Remove edges which link two vertices
     * \return number of removed edges
     */
    virtual size_t removeEdges(const Vertex::Ptr& a, const Vertex::Ptr& b);

    /**
     * Test if graph contains this edge
     * \return true if graph contains edge, false otherwise
     */
    bool contains(const Edge::Ptr& edge) const;

    /**
     * Test if graph contains this vertex
     * \return true if graph contains vertex, false otherwise
     */
    bool contains(const Vertex::Ptr& vertex) const;

    /**
     * \brief Get the edge id for this graph
     * \throw std::runtime_error if the edge is not part of this graph
     */
    GraphElementId getEdgeId(const Edge::Ptr& edge) const { (void) edge; return dynamic_pointer_cast<GraphElement>(edge)->getId( getId() ); }

    /**
     * \brief Get the edge by id
     * \return Edge with given id
     */
    virtual Edge::Ptr getEdge(GraphElementId id) const { (void) id; throw std::runtime_error("BaseGraph::getEdge: not implemented"); }

    /**
     * \brief Get edge by given vertices
     * \return List of edges that start at source and end at target for directed
     * graphs -- for undirected, return all edges
     */
    virtual std::vector<Edge::Ptr> getEdges(const Vertex::Ptr& source, const Vertex::Ptr& target) const;

    /**
     * \brief Get edges by given vertices and return only edges of a given type
     * that end at target
     * Type should be a subclass of Edge!!
     * If dynamic_pointer_cast returns null pointer, object is not added
     */
    template<typename T>
    std::vector< shared_ptr<T> > getEdges(const Vertex::Ptr& source, const Vertex::Ptr& target) const
    {
        std::vector< shared_ptr<T> > edges;
        EdgeIterator::Ptr edgeIt = getEdgeIterator(source);
        while(edgeIt->next())
        {
            shared_ptr<T> edge = dynamic_pointer_cast<T>( edgeIt->current() );
            if(edge && edge->getTargetVertex() == target)
            {
                edges.push_back(edge);
            }
        }

        return edges;
    }

    /**
     * Get the graph id
     * \return id
     */
    GraphId getId() const { return mId; }

    /**
     * Get the iterator over all vertices in this graph
     * \return the vertex iterator
     */
    virtual VertexIterator::Ptr getVertexIterator() const { throw std::runtime_error("graph_analysis::BaseGraph::getVertexIterator has not been implemented"); }
    virtual BaseIterable<VertexIterator::Ptr,BaseGraph, Vertex::Ptr> vertices() { return BaseIterable<VertexIterator::Ptr,BaseGraph, Vertex::Ptr>(this,&BaseGraph::getVertexIterator); }

    /**
     * Get iterator over all edges in this graph
     * \return the edge iterator
     */
    virtual EdgeIterator::Ptr getEdgeIterator() const { throw std::runtime_error("graph_analysis::BaseGraph::getEdgeIterator has not been implemented"); }
    virtual BaseIterable<EdgeIterator::Ptr,BaseGraph, Edge::Ptr> edges() { return BaseIterable<EdgeIterator::Ptr,BaseGraph, Edge::Ptr>(this,&BaseGraph::getEdgeIterator); }

    /**
     * Retrieve all vertices
     * This default implementation uses the iterator, thus requires O(|vertices|)
     * \return all vertices in the graph
     */
    virtual std::vector<Vertex::Ptr> getAllVertices() const;

    /**
     * Retrieve all edges
     * The default implementation uses the iterator, thus requires O(|edges|)
     * \return all edges in the graph
     */
    virtual std::vector<Edge::Ptr> getAllEdges() const;

    /**
     * Get iterator over all edges (in/out) from the given vertex
     * \return EdgeIterator over all edges of this vertex
     */
    virtual EdgeIterator::Ptr getEdgeIterator(const Vertex::Ptr& vertex) const { (void) vertex; throw std::runtime_error("BaseGraph::getEdgeIterator: not implemented"); }

    /**
     * Apply filters to base graph
     * \return subgraph with filters applied
     */
    static SubGraph::Ptr applyFilters(const Ptr& graph, const Filter<Vertex::Ptr>::Ptr& vertexFilter, const Filter<Edge::Ptr>::Ptr& edgeFilter);

    /**
     * Clear the graph from all nodes and edges
     */
    void clear();

    /**
     * Get size of the graph, i.e. by definition the number of edges in the
     * graph
     */
    size_t size() const { return getAllEdges().size(); }

    /**
     * Get the order of the graph, i.e. by definition the number of vertices in
     * the graph
     */
    size_t order() const { return getAllVertices().size(); }

    /**
     * Get out edge iterator
     * If the graph is undirected will return the same iterator as getEdgeIterator
     */
    virtual EdgeIterator::Ptr getOutEdgeIterator(const Vertex::Ptr& vertex) const { return getEdgeIterator(vertex); }

    /**
     * Get in edge iterator
     * If the graph is undirected will return the same iterator as getEdgeIterator
     */
    virtual EdgeIterator::Ptr getInEdgeIterator(const Vertex::Ptr& vertex) const { return getEdgeIterator(vertex); }

    SpecializedIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr,Vertex::Ptr> inEdges(const Vertex::Ptr& vertex) const;
    SpecializedIterable<EdgeIterator::Ptr, BaseGraph, Edge::Ptr,Vertex::Ptr> outEdges(const Vertex::Ptr& vertex) const;

    /**
     * Test if graph is undirected
     */
    bool isUndirected() const { return !isDirected(); }

    /**
     * Test if graph is directed
     */
    bool isDirected() const { return mDirected; }

protected:

    /**
     * Default constructor for a graph
     */
    BaseGraph(ImplementationType type, bool directed);

    /**
     * Add a vertex from the internal graph representation
     * \return Element id of this vertex within this graph
     */
    virtual GraphElementId addVertexInternal(const Vertex::Ptr&) { throw std::runtime_error("BaseGraph::addVertexInternal: not implemented"); }

    /**
     * Remove a vertex from the internal graph representation
     * \return Element id of this vertex within this graph
     */
    virtual void removeVertexInternal(const Vertex::Ptr&) { throw std::runtime_error("BaseGraph::removeVertexInternal: not implemented"); }

    /**
     * Add an edge using source and target vertex on the internal
     * graph representation
     * \return Element id of this edge within this graph
     */
    virtual GraphElementId addEdgeInternal(const Edge::Ptr& edge, GraphElementId sourceVertexId, GraphElementId edgeVertexId) { (void) edge; (void) sourceVertexId; (void) edgeVertexId; throw std::runtime_error("BaseGraph::addEdgeInternal: not implemented"); }

    /**
     * Remove an edge from the internal graph representation
     * \return Element id of this vertex within this graph
     */
    virtual void removeEdgeInternal(const Edge::Ptr&) { throw std::runtime_error("BaseGraph::removeEdgeInternal: not implemented"); }

    /**
     * Create subgraph of the given baseGraph
     * \param baseGraph BaseGraph that this subgraph is related to
     */
    virtual SubGraph::Ptr createSubGraph(const Ptr& baseGraph) const { (void) baseGraph; throw std::runtime_error("BaseGraph::createSubGraph: not implemented"); }

private:
    /// Id of the graph
    GraphId mId;
    /// General id counter to assign unique graph ids
    static GraphId msId;
    ImplementationType mImplementationType;

    // Graph attribute
    bool mDirected;

    // The current hook
    std::set<BaseGraphObserver::Ptr> mObservers;

    // Notification of observers
    void notifyAll(const Vertex::Ptr& vertex, const EventType& event);
    void notifyAll(const Edge::Ptr& edge, const EventType& event);
};

} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_BASE_GRAPH_HPP
