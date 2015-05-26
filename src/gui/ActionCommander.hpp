#ifndef GRAPH_ANALYSIS_GUI_ACTIONCOMMANDER_HPP
#define GRAPH_ANALYSIS_GUI_ACTIONCOMMANDER_HPP

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
    ActionCommander(const GraphWidget* graphWidget);
    ~ActionCommander();
    QAction* addAction(const char *title, const char *slot);
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg);
private:
    const GraphWidget* mpGraphWidget;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_ACTIONCOMMANDER_HPP
