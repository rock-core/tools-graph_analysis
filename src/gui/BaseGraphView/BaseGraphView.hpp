#ifndef GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_HPP
#define GRAPH_ANALYSIS_GUI_BASEGRAPHVIEW_HPP

#include "../GraphWidget.hpp"

#include "../VertexItemBase.hpp"
#include "../EdgeItemBase.hpp"

namespace graph_analysis
{
namespace gui
{

class GraphLayout;

/**
 * \file BaseGraphView.hpp
 * \class BaseGraphView
 * \brief displaying the raw hypergraph in all its glory
 * \details maintains and displays a simple view of its BaseGraph
 *
 * In order to select layouts for the graph you can use the following scheme
 * \verbatim

  BaseGraphView* mpBaseGraphView = new ...

  bool ok;
  QString desiredLayout = QInputDialog::getItem(this, tr("Select Layout"),
                              tr("select a layout:"), mpBaseGraphView->getSupportedLayouts(),
                              0, false, &ok);
  if(ok)
  {
      mpBaseGraphView->applyLayout(desiredLayout.toStdString());
  }

  mpBaseGraphView->updateVisualization();

 \endverbatim
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
     * \param parent Parent widget (if there is one)
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

    /**
     * Set a layout name
     */
    void applyLayout(const std::string& layoutName = "");

    virtual QStringList getSupportedLayouts() const;
};

} // end namespace gui
} // end namespace graph_analysis
#endif
