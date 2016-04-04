#ifndef GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_HPP
#define GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_HPP

#include <graph_analysis/gui/GraphWidget.hpp>

#include <graph_analysis/gui/VertexItemBase.hpp>
#include <graph_analysis/gui/EdgeItemBase.hpp>

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
    BaseGraphView(graph_analysis::BaseGraph::Ptr graph, QWidget *parent = NULL);
    virtual ~BaseGraphView();

    QString getClassName() const;

    /**
     * deletes all internal information and rebuilds the visualization
     * according to the current BaseGraph. Respawns all graphical elements by
     * the underlying base graph and trigger the layouting
     */
    void populateCanvas();

};

} // end namespace gui
} // end namespace graph_analysis
#endif
