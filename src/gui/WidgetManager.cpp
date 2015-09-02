
#include "WidgetManager.hpp"
#include "GraphWidgetManager.hpp"
#include "LayerViewWidget.hpp"
#include "ComponentEditorWidget.hpp"

#include <exception>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

WidgetManager::WidgetManager()
    : mpGraphWidgetManager(NULL)
    , mpMainWindow(NULL)
    , mpComponentEditorWidget(NULL)
    , mpLayerViewWidget(NULL)
    , mpStackedWidget(NULL)
    , mpPropertyDialog(NULL)
{
}

WidgetManager::~WidgetManager()
{
}

void WidgetManager::setWidget(GraphWidget* widget)
{
    if(dynamic_cast<ComponentEditorWidget*>(widget))
    {
        setComponentEditorWidget(dynamic_cast<ComponentEditorWidget*>(widget));
    } else if (dynamic_cast<LayerViewWidget*>(widget))
    {
        setLayerViewWidget(dynamic_cast<LayerViewWidget*>(widget));
    } else {
        throw std::invalid_argument("graph_analyis::gui::WidgetManager::setWidget: unsupported widget type");
    }
}

GraphWidgetManager* WidgetManager::getGraphWidgetManager(void)
{
    if(!mpGraphWidgetManager)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getGraphWidgetManager: mpGraphWidgetManager has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpGraphWidgetManager;
}
void WidgetManager::setGraphWidgetManager(GraphWidgetManager *graphManager)
{
    mpGraphWidgetManager = graphManager;
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

LayerViewWidget* WidgetManager::getLayerViewWidget(void)
{
    if(!mpLayerViewWidget)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getLayerViewWidget: mpLayerViewWidget has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpLayerViewWidget;
}
void WidgetManager::setLayerViewWidget(LayerViewWidget *layerWidget)
{
    mpLayerViewWidget = layerWidget;
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
