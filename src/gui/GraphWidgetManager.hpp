#ifndef GRAPH_ANALYSIS_GUI_GRAPH_WIDGET_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_GRAPH_WIDGET_MANAGER_HPP

#include <string>
#include <sstream>
#include <QObject>
#include <QString>
#include <QStatusBar>
#include <QDockWidget>
#include <QMainWindow>
#include <QTabWidget>
#include "../GraphIO.hpp"

namespace graph_analysis {
namespace gui {

class GraphWidget;

/**
 * \file GraphWidgetManager.hpp
 * \class GraphWidgetManager
 * \brief The GraphWidgetManager loads and connect all main graph widget components
 * \details manages graph widgets under a common stack widget as well as the
 *          main window and the dockable properties dialog and other
 *          housekeeping e.g. GUI icons
 *
 * this is the main-qobject in the hierarchy, which binds all higher-level
 * editor/visualization widgets of graph_analysis::GraphWidget to the currently loaded
 * instance of graph_analysis::BaseGraph. This class also handles importing and
 * exporting from a file.
 *
 * this thing also provides a statusbar-like mechanism, where proper widgets
 * can display information for the user about the current state.
 *
 * this class provides all actions, menus and other mode-elements
 *
 * this class also provides the GraphFilter elements to allow restricting
 * editing operations.
 */
class GraphWidgetManager : public QObject
{
    Q_OBJECT
public:
    enum Mode { CONNECT_MODE, EDIT_MODE, MOVE_MODE };

    /// StatusBar standard timeout (in ms)
    static const int TIMEOUT;

    GraphWidgetManager();

    /// destructor - frees the main window (which recursively frees all other Qt components)
    ~GraphWidgetManager();

    /// prints verbose help message regarding usage of 'cmd' to string stream 'ss'
    void helpSetup(std::stringstream& ss, const std::string& cmd = std::string("program"));

    void updateStatus(const char* message, int timeout = 0)
    {
        mpStatus->showMessage(QString(message), timeout);
    }

    /// setter method for updating the temporary text (i.e. for another 'timeout' miliseconds) currently being displayed on the Status Bar to the given 'message'
    void updateStatus(const std::string& message, int timeout = 0)
    {
        mpStatus->showMessage(QString(message.c_str()), timeout);
    }

    /// setter method for updating the temporary text (i.e. for another 'timeout' miliseconds) currently being displayed on the Status Bar to the given 'message'
    void updateStatus(const QString& message, int timeout = 0)
    {
        mpStatus->showMessage(message, timeout);
    }

public slots:
    /**
     * @brief triggers a refresh() in the currently displayed graph widget
     *
     * this will re-layout the whole graph
     */
    void refresh();
    /// triggers shuffle() in the currently displayed graph widget
    void shuffle();

    void addGraphWidget(GraphWidget* graphWidget);

    void resetGraph(bool keepData = false);

    void importGraph();
    void exportGraph();

    /**
     * Handle tab changes
     */
    void tabChanged(int index) { refresh(); }


protected:
    /// conceptual underlying graph
    graph_analysis::BaseGraph::Ptr mpGraph;

    void notifyModeChange(Mode mode);
    void notifyAll();

private:
    /// main window of the qt application
    QMainWindow *mpMainWindow;
    /**
     * stacked widget to toggle between different available views, like the
     * diagram editor and layers viewer.
     */
    QTabWidget* mpTabWidget;
    /// status bar
    QStatusBar* mpStatus;
    /// the layouting engine to be used when initializing the graph widgets
    QString mLayout;

    GraphWidget* currentGraphWidget();

    /// currently used editing mode. will be propagated to all available views.
    Mode mMode;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_GUI_GRAPH_WIDGET_MANAGER_HPP
