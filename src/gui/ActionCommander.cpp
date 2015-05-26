
#include "ActionCommander.hpp"
#include "GraphWidget.hpp"

namespace graph_analysis {
namespace gui {

ActionCommander::ActionCommander(const GraphWidget* graphWidget)
: mpGraphWidget(graphWidget)
{}

ActionCommander::~ActionCommander()
{}

QAction* ActionCommander::addAction(const char *title, const char *slot)
{
    QAction *action = new QAction(QString(title), (QObject*) mpGraphWidget);
    bool connected = mpGraphWidget->connect(action, SIGNAL(triggered()), mpGraphWidget, slot);
    if(!connected)
    {
        std::string error = std::string("Failed to connect action ") + std::string(title) + " to the GraphWidget context menu";
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addMappedAction(const char *title, const char *slot, QObject *arg)
{
    QSignalMapper* signalMapper = new QSignalMapper((QObject*) mpGraphWidget);
    QAction *action = new QAction(QString(title), (QObject*) mpGraphWidget);
    mpGraphWidget->connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(action, arg);
    bool connected = mpGraphWidget->connect(signalMapper, SIGNAL(mapped(QObject*)), mpGraphWidget, slot);
    if(!connected)
    {
        std::string error = std::string("Failed to connect action ") + std::string(title) + " to the GraphWidget context menu";
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

} // end namespace gui
} // end namespace graph_analysis
