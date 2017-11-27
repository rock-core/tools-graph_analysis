#ifndef GRAPH_ANALYSIS_GUI_HPP
#define GRAPH_ANALYSIS_GUI_HPP

#include <QMainWindow>
#include "QBaseGraph.hpp"
#include <QSettings>

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
    explicit GraphAnalysisGui( BaseGraph::Ptr graph );
    ~GraphAnalysisGui();

private:
    // gui elements
    Ui::GraphAnalysisGui* mpUi;

    // the basegraph where the subwidgets will work upon
    QBaseGraph* mpQBaseGraph;
    BaseGraph::Ptr mpBaseGraph;

    // the subwigets, present in this window
    BaseGraphView* mpBaseGraphView;

    QList<QAction*> mpRecentFileActions;
    enum { MaxRecentFiles = 5 };

    void init();

    void notifyAll();

    /**
     * Store a recent import file list in the setting
     * 'recentImportFileList'
     */
    void updateRecentFileActions();

    QString strippedName(const QString& fullFileName);

    void activateGraph(graph_analysis::BaseGraph::Ptr& graph);

public slots:
    void importGraph();
    void exportGraph();
    void selectLayout();
    void saveScene();

    void playGraph();
    void reversePlayGraph();
    void importRecentFile();

private slots:
    /*Connected to QBaseGraph*/
    void updateVisualization();
    void on_tabWidget_currentChanged(int index);

};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_HPP
