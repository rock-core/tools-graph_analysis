#ifndef GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H
#define GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H

#include <map>
#include <QMainWindow>
#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/gui/NodeItem.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>
#include <graph_analysis/gui/layer_view/LayerViewWidget.hpp>

namespace graph_analysis {
namespace io {
class GVGraph;
} // end namespace io

namespace gui {

class NodeItem;
class EdgeItem;
class ActionCommander;

/**
 * \file ComponentEditorWidget.hpp
 * \class ComponentEditorWidget
 * \brief component editor graph widget implementation
 * \details maintains and displays a diagram with clusternodes, their features and the links (edges) between their ports
 */
class ComponentEditorWidget : public GraphWidget
{
    Q_OBJECT
public:
    /**
     * \brief constructor
     * \param parent qt parent widget
     */
    ComponentEditorWidget(QWidget *parent = NULL);
    /// destructor
    virtual ~ComponentEditorWidget();

    static QString getName() { return "graph_analysis::gui::ComponentEditor"; }
    virtual QString getClassName() const { return getName(); }

    void registerTypes();

//    /**
//     * \brief introduces (creates + adds) a new vertex in the basegraph and enables it
//     * \param type requested type for the new vertex
//     * \param label requested label for the new vertex
//     * \return smart pointer to the newly created vertex instance
//     */
//    Vertex::Ptr createVertex(const std::string& type, const std::string& label = std::string());
//    /**
//     * \brief introduces (creates + adds) a new edge in the basegraph and enables it
//     * \param sourceNode requested source endpoint for the new edge
//     * \param targetNode requested target endpoint for the new edge
//     * \param label requested label for the new edge
//     * \return smart pointer to the newly created vertex instance
//     */
//    Edge::Ptr createEdge(Vertex::Ptr sourceNode, Vertex::Ptr targetNode, const std::string& label = std::string());

    virtual void updateLayout();

public slots:
    /// shuffles all the nodes in the diagram graph editor
    void shuffle();

    void addFeatureDialog();

    /// displays context menu (upon a right click in the scene)
    void showContextMenu(const QPoint& pos);

protected:

    items::Feature* getFeature(const GraphElement::Ptr& element);
private:
    NodeItem* mpLastFocusedNodeItem;
    // Allow to map a graph element to its feature object (if it is a feature)
    std::map<GraphElement::Ptr, items::Feature*> mFeatureMap;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_COMPONENTEDITORWIDGET_H
