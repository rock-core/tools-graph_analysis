#ifndef GRAPH_ANALYSIS_IO_GVGRAPH_HPP
#define GRAPH_ANALYSIS_IO_GVGRAPH_HPP

#include <set>
#include <graphviz/gvc.h>
#include <graph_analysis/BaseGraph.hpp>

// http://graphviz.org/doc/libguide/libguide.pdf
// The agopen method for opening a graph
//
// AGRAPH, AGDIGRAPH, AGRAPHSTRICT, AGDIGRAPHSTRICT
// version >= 2.36
// Agundirected, Agstrictundirected, Agdirected, Agstrictdirected
static inline Agraph_t* _agopen(const std::string& name, Agdesc_t kind)
{
    // extern Agraph_t *agopen(char *name, Agdesc_t desc, Agdisc_t * disc);
    // first arg: graph
    // second arg: type of graph
    // third arg: (optional) reference to functions for reading, memory, etc. --
    //    0 for default
    return agopen(const_cast<char*>(name.c_str()), kind, 0);
}

static inline Agnode_t* _agnode(Agraph_t* g, const std::string& name)
{
    return agnode(g, const_cast<char*>(name.c_str()), true);
}

static inline Agedge_t* _agedge(Agraph_t* g, Agnode_t* node0, Agnode_t* node1, const std::string& name, bool create)
{
    return agedge(g, node0, node1, const_cast<char*>(name.c_str()), create);
}

static inline Agedge_t* _agidedge(Agraph_t* g, Agnode_t* node0, Agnode_t* node1, graph_analysis::GraphElementId id, bool create)
{
    return agidedge(g, node0, node1, id, create);
}

namespace graph_analysis {
namespace io {

// A struct containing the information for a GVGraph's node
// This implementation has been made based on Steve D. Lazaro's presentation on graphviz integration into qt
// \see http://www.mupuf.org/blog/2010/07/08/how_to_usempGraphviz_to_drawmGraphs_in_a_qtmGraphics_scene/
// \credits Steve D. Lazaro, 2010
//
class GVNode
{
public:
    GVNode()
    {}

    Vertex::Ptr getVertex() const { return mpVertex; }
    const std::string& getName() const { return mName; }

    int x() const { return mCenterPosX; }
    int y() const { return mCenterPosY; }
    int height() const { return mHeight; }
    int width() const { return mWidth; }

    GVNode(Vertex::Ptr vertex, const std::string& name, int x, int y, double height, double width)
        : mpVertex(vertex)
        , mName(name)
        , mCenterPosX(x)
        , mCenterPosY(y)
        , mHeight(height)
        , mWidth(width)
    {}

private:
    graph_analysis::Vertex::Ptr mpVertex;

    /// The unique identifier of the node in the graph
    std::string mName;

    /// The position of the center point of the node from the top-left corner
    int mCenterPosX, mCenterPosY;

    /// The size of the node in pixels
    double mHeight, mWidth;
};

/// A struct containing the information for a GVGraph's edge
class GVEdge
{
public:
    GVEdge(Edge::Ptr edge)
        : mpEdge(edge)
    {}

    Edge::Ptr getEdge() const { return mpEdge; }

private:
    Edge::Ptr mpEdge;

    ///**
    // * \brief Path of the edge's line
    // * \return edge's path
    // */
    //QPainterPath path;
};

/**
 * \class GVGraph
 * \brief An object containing a libgraph graph and its associated nodes and edges
 */
class GVGraph
{
public:
    /// Default DPI value used by dot (which uses points instead of pixels for coordinates)
    static const double DotDefaultDPI;

    static std::set<std::string> getSupportedLayouts() { return msSupportedLayouts; }

    /*!
     * \brief Construct a Graphviz graph object
     * \param name The name of the graph, must be unique in the application
     * \param node_size The size in pixels of each node
     */
    GVGraph(BaseGraph::Ptr baseGraph, const std::string& name, double node_size = 0);

    /**
     *
     */
    ~GVGraph();

    /**
     * Set the graphviz based graph attribute by name and value
     * \param name Name of attribute
     * \param value Value for this attribute
     * \return status
     */
    int setGraphAttribute(const std::string& name, const std::string& value);

    /**
     * Get the graphviz based graph attribute
     * \param name Name of attribute
     * \param defaultValue The return value will be this if the attribute has not been set for the graph
     * \return Current or default value for this attribute
     */
    std::string getGraphAttribute(const std::string& name, const std::string& defaultValue = "") const;

    /**
     * Set a node attribute by name
     * \param name Name of node's attribute
     * \param value Value to set
     */
    void setNodeAttribute(const std::string& name, const std::string& value);

    /**
     * Set a edge attribute by name
     * \param name Name of edge's attribute
     * \param value Value to set
     */
    void setEdgeAttribute(const std::string& name, const std::string& value);

    /// Add and remove nodes
    GraphElementId addNode(graph_analysis::Vertex::Ptr vertex);
    void addNodes(const std::vector<Vertex::Ptr>& names);
    void removeNode(graph_analysis::Vertex::Ptr vertex);
    void removeNode(GraphElementId id);
    void clearNodes();

    /// Add and remove edges
    GraphElementId addEdge(Edge::Ptr edge);
    void removeEdge(Edge::Ptr edge);
    void removeEdge(GraphElementId id);
    void clearEdges();

    /// Set the font to use in all the labels
    void setFont(const std::string& fontname, uint16_t fontsizeInPt);

    /**
     *  Apply a layout to the graph
     *  layouts: circo, dot, fdp, neato, osage, sfdp, twopi
     *  \see http://www.graphviz.org/Documentation.php for available layouts
     */
    void applyLayout(const std::string& layout = "dot");
    void renderToFile(const std::string& filename, const std::string& layout = "dot", bool forced = false);

    boxf boundingRect() const;
    void setRootNode(Vertex::Ptr vertex);

    std::vector<GVNode> nodes() const;
    std::vector<GVEdge> edges() const;

    GraphElementId getId(Agnode_t* node) const;
    GraphElementId getId(Agedge_t* edge) const;

    std::string getUniqueName(Vertex::Ptr vertex) const;
    std::string getUniqueName(Edge::Ptr edge) const;

private:
    GVC_t* mpContext;
    Agraph_t* mpGVGraph;
    BaseGraph::Ptr mpBaseGraph;

    std::map<GraphElementId, Agnode_t*> mNodes;
    std::map<GraphElementId, Agedge_t*> mEdges;

    /// DPI setting
    double mDPI;
    /// ScalingFactor based on assumed screen resolution and graphviz DPI setting
    double mScalingFactor;
    bool mAppliedLayout;
    static std::set<std::string> msSupportedLayouts;
    static GraphElementId msEdgeId;
};

} // end namespace io
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_IO_GVGRAPH_HPP
