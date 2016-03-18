#ifndef GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_HPP
#define GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_HPP

#include <QGraphicsView>
#include <graph_analysis/Graph.hpp>
#include <graph_analysis/Filter.hpp>
#include <graph_analysis/gui/GraphWidget.hpp>

namespace graph_analysis
{
namespace gui
{

/**
 * \file BaseGraphView.hpp
 * \class BaseGraphView
 * \brief displaying the raw hypergraph in all its glory
 * \details maintains and displays a simple view of its BaseGraph
 *
 * treats the BaseGraph as read-only.
 */
class BaseGraphView : public GraphWidget
{
    Q_OBJECT

  public:
    /**
     * \brief constructor
     * \param graph underlying base graph
     */
    BaseGraphView(QWidget *parent = NULL);
    /// destructor
    virtual ~BaseGraphView();

    QString getClassName() const;

    /// deletes all internal information; disregards keepData as the underlying
    /// base graph is treated as read-only (except by its dedicated setter
    /// method)
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
#endif
