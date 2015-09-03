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

/**
 * \file ActionCommander.hpp
 * \class ActionCommander
 * \brief Creates QAction-s and registers them with given slots of the supplied QObject
 * \details used to partially automate the code refactoring for the QMenu construction at runtime
 */
class ActionCommander
{
public:
    /// constructor; stores the QObject instance reference for qt signal-slot connections
    ActionCommander(QObject* object);
    /// destructor
    ~ActionCommander();

    /**
     * \brief creates and registers a basic action with the stored 'mpObject' qt object
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \return pointer to the resulting QAction instance
     */
    QAction* addAction(const char *title, const char *slot);
    /**
     * \brief creates and registers a basic action with the provided 'object' instead of the mpObject member field
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \param object qt object to register to
     * \return pointer to the resulting QAction instance
     */
    QAction* addAction(const char *title, const char *slot, QObject* object);
    /**
     * \brief creates and registers a basic action with the stored 'mpObject' qt object
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \param icon small image to be displayed left of the action title
     * \return pointer to the resulting QAction instance
     */
    QAction* addAction(const char *title, const char *slot, const QIcon& icon);
    /**
     * \brief creates and registers a basic action with the provided 'object' instead of the mpObject member field
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \param icon small image to be displayed left of the action title
     * \param object qt object to register to
     * \return pointer to the resulting QAction instance
     */
    QAction* addAction(const char *title, const char *slot, const QIcon& icon, QObject* object);

    /**
     * \brief creates and registers a basic action with the provided 'object' instead of the mpObject member field
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \param icon small image to be displayed left of the action title
     * \param keySequence Sequence of keys to trigger this action
     * \param statusTip Text that shows up as status tip
     * \param object qt object to register to
     * \return pointer to the resulting QAction instance
     * \see http://doc.qt.io/qt-4.8/qkeysequence.html
     */
    QAction* addAction(const char *title, const char *slot, const QIcon& icon, const QKeySequence& keySequence, const QString& statusTip = QString(), QObject* object = 0);

    /**
     * \brief creates and registers a complex action (with 1 parameter) with the stored 'mpObject' qt object
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \param arg pointer to the argument of this action (it has to be a subclass of QObject)
     * \return pointer to the resulting QAction instance
     */
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg);
    /**
     * \brief creates and registers a complex action (with 1 parameter) with the provided 'object' instead of the mpObject member field
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \param arg pointer to the argument of this action (it has to be a subclass of QObject)
     * \param object qt object to register to
     * \return pointer to the resulting QAction instance
     */
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg, QObject* object);
    /**
     * \brief creates and registers a complex action (with 1 parameter) with the stored 'mpObject' qt object
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \param arg pointer to the argument of this action (it has to be a subclass of QObject)
     * \param icon small image to be displayed left of the action title
     * \return pointer to the resulting QAction instance
     */
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg, const QIcon& icon);
    /**
     * \brief creates and registers a complex action (with 1 parameter) with the provided 'object' instead of the mpObject member field
     * \param title text to be displayed for this action
     * \param slot string with the slot function name
     * \param arg pointer to the argument of this action (it has to be a subclass of QObject)
     * \param icon small image to be displayed left of the action title
     * \param object qt object to register to
     * \return pointer to the resulting QAction instance
     */
    QAction* addMappedAction(const char *title, const char *slot, QObject *arg, const QIcon& icon, QObject* object);
private:
    /// qt object to register the produced QAction-s to
    QObject* mpObject;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_ACTIONCOMMANDER_HPP
