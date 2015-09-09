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
#include <graph_analysis/GraphIO.hpp>

namespace graph_analysis {
namespace gui {

class GraphWidget;

namespace dialogs {
    class PropertyDialog;
}

/**
 * \file GraphWidgetManager.hpp
 * \class GraphWidgetManager
 * \brief The GraphWidgetManager loads and connect all main graph widget components
 * \details manages graph widgetsunder a common stack widget
 *      as well as the main window and the dockable properties dialog and other housekeeping e.g. GUI icons
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
    /// re-loads the property dialog/panel in the scene
    void reloadPropertyDialog();
    /// triggers refresh() in the currently displayed graph widget
    void refresh();
    /// triggers shuffle() in the currently displayed graph widget
    void shuffle();
    /// triggers changeLayout() in the currently displayed graph widget
    void selectLayout();
    /// reloads the property dialog/panel if it is not running any more
    void reloadPropertyDialogMainWindow();

    void removeSelection();
    void renameSelection();
    void renameElementDialog(GraphElement::Ptr element);

    void addFeature();
    void swapFeatures();
    void addVertex();

    int addGraphWidget(GraphWidget* graphWidget);

    void resetGraph(bool keepData = false);

    void importGraph();
    void exportGraph();

    void setMoveMode() { setMode(MOVE_MODE); }
    void setConnectMode() { setMode(CONNECT_MODE); }
    void setEditMode() { setMode(EDIT_MODE); }

    void setMode(Mode mode) { mMode = mode; notifyModeChange(mode); }
    Mode getMode() const { return mMode; }

    /**
     * Handle tab changes
     */
    void tabChanged(int index) { refresh(); }


protected:
    /// conceptual underlying graph
    graph_analysis::BaseGraph::Ptr mpGraph;

    void notifyModeChange(Mode mode);
    void notifyAll();

    typedef std::map <std::string, io::Writer*> WriterMap; // maps specific keywords to graph exporters
    typedef std::map <std::string, io::Reader*> ReaderMap; // maps specific keywords to graph importers

    /// io components
    /// export
    WriterMap mWriterMap;
    /// import
    ReaderMap mReaderMap;

    /// renders the base graph to custom yml format file 'filename'
    void toYmlFile(const std::string& filename);
    /// renders the base graph to DOT file 'filename' via mpGVGraph directly
    void gvRender (const std::string& filename);
    /// renders the base graph to DOT file 'filename' via the dot exporter (graph writer)
    void toDotFile(const std::string& filename);
    /// renders the base graph to gexf format file 'filename'
    void toXmlFile(const std::string& filename);
    /**
     * \brief loads base graph from file 'filename' (either from custom yml format, or gexf standard format)
     * \param filename the input file to parse the graph from
     * \param format the string key to use for reader look-up in the readers map
     * \return error code: 0 when successful; non-zero if anything went wrong
     */
    int fromFile(const std::string& filename, const std::string& format);
    /**
     * \brief loads base graph from file 'filename' - tries the 2 available formats: the custom yml and the standard gexf
     * \param file the source filename to parse from
     * \param prefers_gexf boolean flag telling whether to prefer gexf over custom yml in case the filename has no extension
     * \param status boolean flag telling whether to update the status bar
     */
    void fromFile(const QString& file, bool prefers_gexf = true, bool status = true);


private:
    /// main window of the qt application
    QMainWindow *mpMainWindow;
    /// stacked widget to toggle between the diagram editor and layers viewer (the actual central widget of the main window)
    QTabWidget* mpTabWidget;
    /// status bar
    QStatusBar* mpStatus;
    /// the property dialog (a.k.a. command panel) dockable GUI component
    dialogs::PropertyDialog *mpPropertyDialog;
    /// the layouting engine to be used when initializing the graph widgets
    QString mLayout;

    /// Widget that are associated with the main widget
    std::vector<GraphWidget*> mGraphWidgets;

    GraphWidget* currentGraphWidget();

    Mode mMode;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_GUI_GRAPH_WIDGET_MANAGER_HPP
