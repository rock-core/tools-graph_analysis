#include "WidgetManager.hpp"
#include "GraphWidgetManager.hpp"
#include <graph_analysis/gui/dialogs/PropertyDialog.hpp>

#include <exception>
#include <base-logging/Logging.hpp>

namespace graph_analysis {
namespace gui {

WidgetManager::WidgetManager()
    : mpGraphWidgetManager(NULL)
    , mpMainWindow(NULL)
    , mpPropertyDialog(NULL)
{
}

void WidgetManager::setWidget(GraphWidget* widget)
{
    throw std::invalid_argument("graph_analyis::gui::WidgetManager::setWidget: unsupported widget type");
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


dialogs::PropertyDialog* WidgetManager::getPropertyDialog(void)
{
    if(!mpPropertyDialog)
    {
        std::string error_msg("graph_analysis::gui::WidgetManager::getPropertyDialog: mpPropertyDialog has not been initialized!");
        LOG_ERROR_S << error_msg;
        throw std::runtime_error(error_msg);
    }
    return mpPropertyDialog;
}
void WidgetManager::setPropertyDialog(dialogs::PropertyDialog *propertyDialog)
{
    mpPropertyDialog = propertyDialog;
}

} // end namespace gui
} // end namespace graph_analysis
