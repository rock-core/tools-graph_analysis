#include "ActionCommander.hpp"
#include <stdexcept>

namespace graph_analysis {
namespace gui {

ActionCommander::ActionCommander(QObject* object)
: mpObject(object)
{}

ActionCommander::~ActionCommander()
{}

QAction* ActionCommander::addAction(const char *title, const char *slot, QObject* object)
{
    QAction *action = new QAction(QString(title), object);
    bool connected = object->connect(action, SIGNAL(triggered()), object, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addAction: Failed to connect action ") + std::string(title) + " to the widget menu";
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addAction(const char *title, const char *slot, const QIcon& icon, QObject* object)
{
    QAction *action = new QAction(QString(title), object);
    action->setIcon(icon);
    action->setIconVisibleInMenu(true);
    bool connected = object->connect(action, SIGNAL(triggered()), object, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addAction: Failed to connect action ") + std::string(title) + " to the widget menu";
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addAction(const char *title, const char *slot, const QIcon& icon, const QKeySequence& keySequence, const QString& statusTip, QObject* object)
{
    QAction* action;
    if(!object)
    {
        action = addAction(title, slot, icon);
    } else {
        action = addAction(title, slot, icon, object);
    }

    action->setShortcut(keySequence);
    if(statusTip != QString())
    {
        action->setStatusTip(statusTip);
    }
    return action;
}


QAction* ActionCommander::addMappedAction(const char *title, const char *slot, QObject *arg, QObject* object)
{
    QSignalMapper* signalMapper = new QSignalMapper(object);
    QAction *action = new QAction(QString(title), object);
    object->connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(action, arg);
    bool connected = object->connect(signalMapper, SIGNAL(mapped(QObject*)), object, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addMappedAction: Failed to connect action ");
        error += std::string(title);
        error += std::string(" to the widget menu");
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addMappedAction(const char *title, const char *slot, QObject *arg, const QIcon& icon, QObject* object)
{
    QSignalMapper* signalMapper = new QSignalMapper(object);
    QAction *action = new QAction(QString(title), object);
    action->setIcon(icon);
    action->setIconVisibleInMenu(true);
    object->connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(action, arg);
    bool connected = object->connect(signalMapper, SIGNAL(mapped(QObject*)), object, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addMappedAction: Failed to connect action ");
        error += std::string(title);
        error += std::string(" to the widget menu");
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addAction(const char *title, const char *slot)
{
    QAction *action = new QAction(QString(title), mpObject);
    bool connected = mpObject->connect(action, SIGNAL(triggered()), mpObject, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addAction: Failed to connect action ") + std::string(title) + " to the widget menu";
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addAction(const char *title, const char *slot, const QIcon& icon)
{
    QAction *action = new QAction(QString(title), mpObject);
    action->setIcon(icon);
    action->setIconVisibleInMenu(true);
    bool connected = mpObject->connect(action, SIGNAL(triggered()), mpObject, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addAction: Failed to connect action ") + std::string(title) + " to the widget menu";
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addMappedAction(const char *title, const char *slot, QObject *arg)
{
    QSignalMapper* signalMapper = new QSignalMapper(mpObject);
    QAction *action = new QAction(QString(title), mpObject);
    mpObject->connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(action, arg);
    bool connected = mpObject->connect(signalMapper, SIGNAL(mapped(QObject*)), mpObject, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addMappedAction: Failed to connect action ");
        error += std::string(title);
        error += std::string(" to the widget menu");
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

QAction* ActionCommander::addMappedAction(const char *title, const char *slot, QObject *arg, const QIcon& icon)
{
    QSignalMapper* signalMapper = new QSignalMapper(mpObject);
    QAction *action = new QAction(QString(title), mpObject);
    action->setIcon(icon);
    action->setIconVisibleInMenu(true);
    mpObject->connect(action, SIGNAL(triggered()), signalMapper, SLOT(map()));
    signalMapper->setMapping(action, arg);
    bool connected = mpObject->connect(signalMapper, SIGNAL(mapped(QObject*)), mpObject, slot);
    if(!connected)
    {
        std::string error = std::string("graph_analysis::gui::ActionCommander::addMappedAction: Failed to connect action ");
        error += std::string(title);
        error += std::string(" to the widget menu");
        LOG_ERROR_S << error;
        throw std::runtime_error(error);
    }
    return action;
}

} // end namespace gui
} // end namespace graph_analysis
