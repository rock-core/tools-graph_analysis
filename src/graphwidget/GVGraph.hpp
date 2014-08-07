#ifndef OMVIZ_GRAPHWIDGET_GVGRAPH_HPP
#define OMVIZ_GRAPHWIDGET_GVGRAPH_HPP

#include <graphviz/gvc.h>
#include <QFont>
#include <QGraphicsItem>

// The agopen method for opening a graph
// AGRAPH, AGDIGRAPH, AGRAPHSTRICT, AGDIGRAPHSTRICT
static inline Agraph_t* _agopen(QString name, int kind)
{
    return agopen(const_cast<char *>(qPrintable(name)), kind);
}

// Add an alternative value parameter to the method for getting an object's attribute
static inline QString _agget(void *object, QString attr, QString alt=QString())
{
    QString str=agget(object, const_cast<char *>(qPrintable(attr)));

    if(str==QString())
        return alt;
    else
        return str;
}

// Directly use agsafeset which always works, contrarily to agset
static inline int _agset(void *object, QString attr, QString value)
{
    return agsafeset(object, const_cast<char *>(qPrintable(attr)),
                     const_cast<char *>(qPrintable(value)),
                     const_cast<char *>(qPrintable(value)));
}

static inline Agsym_t* _agnodeattr(Agraph_t* graph, QString name, QString value)
{
    return agnodeattr(graph, const_cast<char*>(qPrintable(name)), const_cast<char*>(qPrintable(value)));
}

static inline Agsym_t* _agedgeattr(Agraph_t * g, QString name, QString value)
{
    return agedgeattr(g, const_cast<char*>(qPrintable(name)), const_cast<char*>(qPrintable(value)));
}

static inline Agnode_t* _agnode(Agraph_t* g, QString name)
{
    return agnode(g, const_cast<char*>(qPrintable(name)));
}

namespace omviz {

// A struct containing the information for a GVGraph's node
// \see http://www.mupuf.org/blog/2010/07/08/how_to_use_graphviz_to_draw_graphs_in_a_qt_graphics_scene/
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

    /// Path of the edge's line
    QPainterPath path;
};

/// An object containing a libgraph graph and its associated nodes and edges
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
    GVGraph(QString name, QFont font=QFont(), qreal node_size=50);
    ~GVGraph();

    /// Add and remove nodes
    void addNode(const QString& name);
    void addNodes(const QStringList& names);
    void removeNode(const QString& name);
    void clearNodes();

    /// Add and remove edges
    void addEdge(const QString& source, const QString& target);
    void removeEdge(const QString& source, const QString& target);
    void removeEdge(const QPair<QString, QString>& key);

    /// Set the font to use in all the labels
    void setFont(QFont font);

    void applyLayout();
    QRectF boundingRect() const;
    void setRootNode(const QString& name);

    QList<GVNode> nodes() const;
    QList<GVEdge> edges() const;

private:
    GVC_t *_context;
    Agraph_t *_graph;
    QFont _font;
    QMap<QString, Agnode_t*> _nodes;
    QMap< QPair<QString, QString>, Agedge_t*> _edges;
};

} // end namespace omviz
#endif // OMVIZ_GRAPHWIDGET_GVGRAPH_HPP
