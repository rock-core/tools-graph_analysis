#ifndef GRAPH_ANALYSIS_GUI_QBASEGRAPH_H
#define GRAPH_ANALYSIS_GUI_QBASEGRAPH_H

#include <QObject>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/BaseGraphObserver.hpp>

namespace graph_analysis
{
namespace gui
{

/**
 * \brief This class converts the callback based interface to the base graph
 * into a Qt signal
 * It implements a virtual interface class BaseGraphObserver and registers it
 * to a given or created BaseGraph
 * NOTE:
 * It might be beneficial to add slots which map to the underlying BaseGraph
 * interface
 */
class QBaseGraph : public QObject, public BaseGraphObserver
{
    Q_OBJECT

public:
    QBaseGraph(QObject* parent = NULL);
    QBaseGraph(graph_analysis::BaseGraph::Ptr graph, QObject* parent = NULL);

    ~QBaseGraph();

    virtual QString getClassName() const
    {
        return "graph_analysis::gui::QBaseGraph";
    }

    graph_analysis::BaseGraph::Ptr getBaseGraph() const
    {
        return mpGraph;
    }

private:
    graph_analysis::BaseGraph::Ptr mpGraph;

    // Implemented virtual functions
    void notify(const Vertex::Ptr& vertex, const EventType& event,
                        const GraphId& origin);
    void notify(const Edge::Ptr& edge, const EventType& event,
                        const GraphId& origin);


signals:
    void graphChanged();
};

} // end namespace gui
} // end namespace graph_analysis
#endif
