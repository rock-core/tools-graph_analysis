#ifndef GRAPH_ANALYSIS_GUI_GVGRAPH_HPP
#define GRAPH_ANALYSIS_GUI_GVGRAPH_HPP

#include <graphviz/gvc.h>
#include <QFont>
#include <QGraphicsItem>

// http://graphviz.org/doc/libguide/libguide.pdf
// The agopen method for opening a graph
//
// AGRAPH, AGDIGRAPH, AGRAPHSTRICT, AGDIGRAPHSTRICT
// version >= 2.36
// Agundirected, Agstrictundirected, Agdirected, Agstrictdirected
static inline Agraph_t* _agopen(QString name, Agdesc_t kind)
{
    // extern Agraph_t *agopen(char *name, Agdesc_t desc, Agdisc_t * disc);
    // first arg: graph
    // second arg: type of graph
    // third arg: (optional) reference to functions for reading, memory, etc. -- 
    //    0 for default
    return agopen(const_cast<char *>(qPrintable(name)), kind, 0);
}

static inline Agnode_t* _agnode(Agraph_t* g, QString name)
{
    return agnode(g, const_cast<char*>(qPrintable(name)), true);
}

static inline Agedge_t* _agedge(Agraph_t* g, Agnode_t* node0, Agnode_t* node1, QString name, bool create)
{
    return agedge(g, node0, node1, const_cast<char*>(qPrintable(name)), create);
}

namespace graph_analysis {
namespace gui {

// A struct containing the information for a GVGraph's node
// This implementation has been made based on Steve D. Lazaro's presentation on graphviz integration into qt
// \see http://www.mupuf.org/blog/2010/07/08/how_to_usempGraphviz_to_drawmGraphs_in_a_qtmGraphics_scene/
// \credits Steve D. Lazaro, 2010
//
struct GVNode
{
    /// The unique identifier of the node in the graph
    QString name;

    /// The position of the center point of the node from the top-left corner
    QPoint centerPos;

    /// The size of the node in pixels
    double height, width;
};

/// A struct containing the information for a GVGraph's edge
struct GVEdge
{
    /// The source and target nodes of the edge
    QString source;
    QString target;

    /**
     * \brief Path of the edge's line
     * \return edge's path
     */
    QPainterPath path;

    /**
     * Unique id for this edge based on source and target node names
     * \return unique id
     */
    QString getId() const { return source + "->" + target; }
};

/**
 * \class GVGraph
 * \brief An object containing a libgraph graph and its associated nodes and edges
 */
class GVGraph
{
public:
    /// Default DPI value used by dot (which uses points instead of pixels for coordinates)
    static const qreal DotDefaultDPI;

    /*!
     * \brief Construct a Graphviz graph object
     * \param name The name of the graph, must be unique in the application
     * \param font The font to use for the graph
     * \param node_size The size in pixels of each node
     */
    GVGraph(QString name, QFont font=QFont(), double node_size = 0);
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
    std::string getGraphAttribute(const std::string& name, const std::string& defaultValue ="") const;

    /**
     * Qt based version of getGraphAttribute
     * \return Current or default value for the given attribute
     */
    QString getQGraphAttribute(const QString& name, const QString& defaultValue ="") const { return QString( getGraphAttribute(name.toStdString(), defaultValue.toStdString()).c_str() ); }

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
    void addNode(const QString& name);
    void addNodes(const QStringList& names);
    void removeNode(const QString& name);
    void clearNodes();

    /// Add and remove edges
    void addEdge(const QString& source, const QString& target, const QString& label = "");
    void removeEdge(const QString& source, const QString& target);
    void removeEdge(const QPair<QString, QString>& key);
    void clearEdges();

    /// Set the font to use in all the labels
    void setFont(QFont font);

    /**
     *  Apply a layout to the graph
     *  layouts: circo, dot, fdp, neato, osage, sfdp, twopi
     *  \see http://www.graphviz.org/Documentation.php for available layouts
     */
    void applyLayout(const std::string& layout = "dot");
    void renderToFile(const std::string& filename, const std::string& layout = "dot");

    QRectF boundingRect() const;
    void setRootNode(const QString& name);

    QList<GVNode> nodes() const;
    QList<GVEdge> edges() const;

private:
    GVC_t* mpContext;
    Agraph_t* mpGraph;
    QFont mFont;
    QMap<QString, Agnode_t*> mNodes;
    QMap< QPair<QString, QString>, Agedge_t*> mEdges;

    /// DPI setting
    double mDPI;
    /// ScalingFactor based on assumed screen resolution and graphviz DPI setting
    double mScalingFactor;

    bool mAppliedLayout;
};

} // end namespace gui
} // end namespace graph_analysis 
#endif // GRAPH_ANALYSIS_GUI_GVGRAPH_HPP
