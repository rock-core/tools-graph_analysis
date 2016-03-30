#ifndef GRAPH_ANALYSIS_GUI_HPP
#define GRAPH_ANALYSIS_GUI_HPP

#include <QMainWindow>
#include <graph_analysis/BaseGraph.hpp>

namespace Ui
{
class GraphAnalysisGui;
}

namespace graph_analysis
{
namespace gui
{

class BaseGraphView;
class ComponentGraphEditor;

class GraphAnalysisGui : public QMainWindow
{
    Q_OBJECT

public:
    GraphAnalysisGui();
    ~GraphAnalysisGui();

private:
    Ui::GraphAnalysisGui* mpUi;

    graph_analysis::BaseGraph::Ptr mpGraph;

    BaseGraphView* mpBaseGraphView;
    ComponentGraphEditor* mpComponentGraphEditor;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_HPP
