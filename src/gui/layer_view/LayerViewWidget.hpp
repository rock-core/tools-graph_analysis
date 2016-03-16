#ifndef GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H
#define GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H

#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/GraphView.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis {
namespace gui {

/**
 * \file LayerViewWidget.hpp
 * \class LayerViewWidget
 * \brief layers graph view widget implementation
 * \details maintains and displays a layered view of its base graph (the latter is treated as read-only with the exception of its setter method)
 */
class LayerViewWidget : public GraphWidget
{
    Q_OBJECT

public:
    /**
     * \brief constructor
     * \param graph underlying base graph
     */
    LayerViewWidget(QWidget *parent = NULL);
    /// destructor
    virtual ~LayerViewWidget();

    static QString getName() { return "graph_analysis::gui::LayerView"; }
    virtual QString getClassName() const { return getName(); }

    /// deletes all internal information; disregards keepData as the underlying base graph is treated as read-only (except by its dedicated setter method)

    /// respawns all graphical elements by the underlying base graph
    /// and trigger the layouting
    virtual void updateLayout();

public slots:
    /// shuffles all the nodes in the layers graph view
    void shuffle();

protected:

    void resetLayoutingGraph();
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_LAYERVIEWWIDGET_H
