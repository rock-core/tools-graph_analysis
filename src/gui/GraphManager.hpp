#ifndef GRAPH_ANALYSIS_GUI_GRAPH_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_GRAPH_MANAGER_HPP

#include "ViewWidget.hpp"
#include "LayerWidget.hpp"
#include "PropertyDialog.hpp"

#include <string>
#include <sstream>
#include <QObject>
#include <QString>
#include <QStatusBar>
#include <QDockWidget>
#include <QMainWindow>
#include <QStackedWidget>

namespace graph_analysis {
namespace gui {

/**
 * \file GraphManager.hpp
 * \class GraphManager
 * \brief class to load and connect all main GUI components
 * \details manages the diagram editor widget and the layers view widget under a common stack widget
 *      as well as the main window and the dockable properties dialog and other housekeeping e.g. GUI icons
 */
class GraphManager : public QObject
{
    Q_OBJECT
public:
    /// constructor; if filename is not empty, it tries to load a graph on init from it
    GraphManager(const QString& filename = QString());
    /// destructor - frees the main window (which recursively frees all other Qt components)
    ~GraphManager();

    /// prints verbose help message regarding usage of 'cmd' to string stream 'ss'
    void helpSetup(std::stringstream& ss, const std::string& cmd = std::string("program"));

    /// setter method for updating the temporary text (i.e. for another 'timeout' miliseconds) currently being displayed on the Status Bar to the given 'message'
    void updateStatus(const std::string& message = std::string(), int timeout = 0)
    {
        mpStatus->showMessage(QString(message.c_str()), timeout);
    }

    /// setter method for updating the temporary text (i.e. for another 'timeout' miliseconds) currently being displayed on the Status Bar to the given 'message'
    void updateStatus(const QString& message = QString(), int timeout = 0)
    {
        mpStatus->showMessage(message, timeout);
    }
    /// re-loads the property dialog/panel in the scene
    void reloadPropertyDialog(void);

private:
    /// main window of the qt application
    QMainWindow *mpMainWindow;
    /// stacked widget to toggle between the diagram editor and layers viewer (the actual central widget of the main window)
    QStackedWidget* mpStackedWidget;
    /// the diagram editor widget GUI component
    ViewWidget *mpViewWidget;
    /// the layers viewer widget GUI component
    LayerWidget *mpLayerWidget;
    /// status bar
    QStatusBar* mpStatus;
    /// the property dialog (a.k.a. command panel) dockable GUI component
    PropertyDialog *mpPropertyDialog;
    /// the layouting engine to be used when initializing the graph widgets
    QString mLayout;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_GUI_GRAPH_MANAGER_HPP