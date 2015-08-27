
#include "WidgetManager.hpp"
#include "GraphManager.hpp"

#include <exception>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

WidgetManager::WidgetManager()
    : mpGraphManager(NULL)
    , mpMainWindow(NULL)
    , mpComponentEditorWidget(NULL)
    , mpLayerWidget(NULL)
    , mpStackedWidget(NULL)
    , mpPropertyDialog(NULL)
{
}

WidgetManager::~WidgetManager()
{
}

GraphManager* WidgetManager::getGraphManager(void)
{
    if(!mpGraphManager)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getGraphManager: mpGraphManager has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpGraphManager;
}
void WidgetManager::setGraphManager(GraphManager *graphManager)
{
    mpGraphManager = graphManager;
}

QMainWindow* WidgetManager::getMainWindow(void)
{
    if(!mpMainWindow)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getMainWindow: mpMainWindow has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpMainWindow;
}
void WidgetManager::setMainWindow(QMainWindow *mainWindow)
{
    mpMainWindow = mainWindow;
}

QStackedWidget* WidgetManager::getStackedWidget(void)
{
    if(!mpStackedWidget)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getStackedWidget: mpStackedWidget has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpStackedWidget;
}
void WidgetManager::setStackedWidget(QStackedWidget *stackedWidget)
{
    mpStackedWidget = stackedWidget;
}

ComponentEditorWidget* WidgetManager::getComponentEditorWidget(void)
{
    if(!mpComponentEditorWidget)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getComponentEditorWidget: mpComponentEditorWidget has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpComponentEditorWidget;
}
void WidgetManager::setComponentEditorWidget(ComponentEditorWidget *viewWidget)
{
    mpComponentEditorWidget = viewWidget;
}

LayerWidget* WidgetManager::getLayerWidget(void)
{
    if(!mpLayerWidget)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getLayerWidget: mpLayerWidget has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpLayerWidget;
}
void WidgetManager::setLayerWidget(LayerWidget *layerWidget)
{
    mpLayerWidget = layerWidget;
}

PropertyDialog* WidgetManager::getPropertyDialog(void)
{
    if(!mpPropertyDialog)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getPropertyDialog: mpPropertyDialog has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpPropertyDialog;
}
void WidgetManager::setPropertyDialog(PropertyDialog *propertyDialog)
{
    mpPropertyDialog = propertyDialog;
}

} // end namespace gui
} // end namespace graph_analysis
