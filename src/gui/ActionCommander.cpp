
#include "ActionCommander.hpp"
#include "GraphWidget.hpp"

namespace graph_analysis {
namespace gui {

ActionCommander::ActionCommander(const QObject* object)
: mpObject(object)
{}

ActionCommander::~ActionCommander()
{}

QAction* ActionCommander::addAction(const char *title, const char *slot)
{
    QAction *action = new QAction(QString(title), (QObject*) mpObject);
    bool connected = mpObject->connect(action, SIGNAL(triggered()), mpObject, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addAction: Failed to connect action ") + std::string(title) + " to the GraphWidget context menu";
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addMappedAction(const char *title, const char *slot, QObject *arg)
{
    QSignalMapper* signalMapper = new QSignalMapper((QObject*) mpObject);
    QAction *action = new QAction(QString(title), (QObject*) mpObject);
    mpObject->connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(action, arg);
    bool connected = mpObject->connect(signalMapper, SIGNAL(mapped(QObject*)), mpObject, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addMappedAction: Failed to connect action ");
        error += std::string(title);
        error += std::string(" to the GraphWidget context menu");
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

} // end namespace gui
} // end namespace graph_analysis
