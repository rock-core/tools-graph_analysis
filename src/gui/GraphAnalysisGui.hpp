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
    namespace task_graph
    {
        class TaskTemplateContainer;
    }
namespace gui
{

class BaseGraphView;
class TaskGraphEditor;

class GraphAnalysisGui : public QMainWindow
{
    Q_OBJECT

public:
    GraphAnalysisGui();
    ~GraphAnalysisGui();

private:
    // gui elements
    Ui::GraphAnalysisGui* mpUi;

    // the basegraph where the subwidgets will work upon
    graph_analysis::BaseGraph::Ptr mpGraph;

    // the subwigets, present in this window
    BaseGraphView* mpBaseGraphView;
    TaskGraphEditor* mpTaskGraphEditor;

    task_graph::TaskTemplateContainer* mpTaskTemplContainer;

    void importGraph();
    void exportGraph();
    void fromFile(const std::string& filename);
    void notifyAll();
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_HPP
