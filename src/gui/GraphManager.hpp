#ifndef GRAPH_ANALYSIS_GUI_GRAPH_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_GRAPH_MANAGER_HPP

#include "ViewWidget.hpp"
#include "LayerWidget.hpp"
#include "PropertyDialog.hpp"

#include <string>
#include <sstream>
#include <QString>
#include <QDockWidget>
#include <QMainWindow>

namespace graph_analysis {
namespace gui {

/**
 * \file GraphManager.hpp
 * \class GraphManager
 * \brief class to load and connect all main GUI components
 * \details manages the diagram editor widget and the layers view widget under a common stack widget
 *      as well as the main window and the dockable properties dialog and other housekeeping e.g. GUI icons
 */
class GraphManager
{
public:
    /// constructor; if filename is not empty, it tries to load a graph on init from it
    GraphManager(const std::string& filename = std::string());
    /// destructor - frees the main window (which recursively frees all other Qt components)
    ~GraphManager();

    /// prints verbose help message regarding usage of 'cmd' to string stream 'ss'
    void helpSetup(std::stringstream& ss, const std::string& cmd = std::string("program"));

private:
    /// the diagram editor widget GUI component
    ViewWidget *mpViewWidget;
    /// the layers viewer widget GUI component
    LayerWidget *mpLayerWidget;
    /// the property dialog (a.k.a. command panel) dockable GUI component
    PropertyDialog *mpPropertyDialog;
    /// main GUI window
    QMainWindow *mpMainWindow;
    /// the layouting engine to be used when initializing the graph widgets
    QString mLayout;
};

} // end namespace gui
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_GUI_GRAPH_MANAGER_HPP