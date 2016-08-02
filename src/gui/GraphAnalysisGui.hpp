#ifndef GRAPH_ANALYSIS_GUI_HPP
#define GRAPH_ANALYSIS_GUI_HPP

#include <QMainWindow>
#include <graph_analysis/gui/QBaseGraph.hpp>

namespace Ui
{
class GraphAnalysisGui;
}

namespace graph_analysis
{
namespace gui
{

class BaseGraphView;

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
    QBaseGraph* mpQBaseGraph;

    // the subwigets, present in this window
    BaseGraphView* mpBaseGraphView;

    void importGraph();
    void exportGraph();
    void fromFile(const std::string& filename);
    void notifyAll();

private slots:
    /*Connected to QBaseGraph*/
    void updateVisualization();
    void on_tabWidget_currentChanged(int index);

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_HPP
