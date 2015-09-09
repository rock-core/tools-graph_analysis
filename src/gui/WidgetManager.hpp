#ifndef GRAPH_ANALYSIS_GUI_WIDGET_MANAGER_HPP
#define GRAPH_ANALYSIS_GUI_WIDGET_MANAGER_HPP

#include <QMainWindow>
#include <QStackedWidget>
#include <base/Singleton.hpp>

namespace graph_analysis {
namespace gui {

    class GraphWidgetManager;
    class GraphWidget;
    class ComponentEditorWidget;
    class LayerViewWidget;

namespace dialogs {
    class PropertyDialog;
}

/**
 * \file WidgetManager.hpp
 * \class WidgetManager
 * \brief Singleton to store handles on all main widgets
 * \details Facilitates links between the main actors (the main GUI components of this qt appplication) in a structured way
 */
class WidgetManager : public base::Singleton<WidgetManager>
{
public:
    /// constructor
    WidgetManager();
    /// destructor
    ~WidgetManager();

    /// getter for the graph GUI manager
    GraphWidgetManager* getGraphWidgetManager(void);
    /// setter for the graph GUI manager
    void setGraphWidgetManager(GraphWidgetManager *graphManager);

    /// getter for the main window of the qt application
    QMainWindow* getMainWindow(void);
    /// setter for the main window of the qt application
    void setMainWindow(QMainWindow *mainWindow);

    /// getter for the diagram editor widget GUI component
    ComponentEditorWidget* getComponentEditorWidget(void);
    /// setter for the diagram editor widget GUI component
    void setComponentEditorWidget(ComponentEditorWidget *viewWidget);

    /// getter for the layers viewer widget GUI component
    LayerViewWidget* getLayerViewWidget(void);
    /// setter for the layers viewer widget GUI component
    void setLayerViewWidget(LayerViewWidget *layerWidget);

    /// getter for the property dialog (a.k.a. command panel) dockable GUI component
    dialogs::PropertyDialog* getPropertyDialog(void);
    /// setter for the property dialog (a.k.a. command panel) dockable GUI component
    void setPropertyDialog(dialogs::PropertyDialog *propertyDialog);

    /// Set widget
    void setWidget(GraphWidget* widget);

private:
    /// graph GUI manager
    GraphWidgetManager *mpGraphWidgetManager;
    /// main window of the qt application
    QMainWindow *mpMainWindow;
    /// the diagram editor widget GUI component
    ComponentEditorWidget *mpComponentEditorWidget;
    /// the layers viewer widget GUI component
    LayerViewWidget *mpLayerViewWidget;
    /// stacked widget to toggle between the diagram editor and layers viewer (the actual central widget of the main window)
    QStackedWidget* mpStackedWidget;
    /// the property dialog (a.k.a. command panel) dockable GUI component
    dialogs::PropertyDialog *mpPropertyDialog;
};


} // end namespace gui
} // end namespace graph_analysis

#endif // GRAPH_ANALYSIS_GUI_WIDGET_MANAGER_HPP
