#ifndef GRAPH_ANALYSIS_GUI_ACTIONCOMMANDER_HPP
#define GRAPH_ANALYSIS_GUI_ACTIONCOMMANDER_HPP

#include <QIcon>
#include <QAction>
#include <QString>
#include <QApplication>
#include <QSignalMapper>
#include <QGraphicsView>
#include <base/Logging.hpp>

namespace graph_analysis {
namespace gui {

    class GraphWidget;

/**
 * @file ActionCommander.hpp
 * @class ActionCommander
 * @brief Creates QAction-s and registers them with given slots
 * @author Petre Munteanu, clasianvmk (at) gmail (dot) com
 * @date 12/05/15
 */
class ActionCommander
{
public:
    ActionCommander(QObject* object);
    ~ActionCommander();
    QAction* addAction(const char *title, const char *slot);
    QAction* addAction(const char *title, const char *slot, QObject* object);
    QAction* addAction(const char *title, const char *slot, const QIcon& icon);
    QAction* addAction(const char *title, const char *slot, const QIcon& icon, QObject* object);
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg);
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg, QObject* object);
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg, const QIcon& icon);
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg, const QIcon& icon, QObject* object);
private:
    QObject* mpObject;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_ACTIONCOMMANDER_HPP
