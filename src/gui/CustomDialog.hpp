#ifndef CUSTOM_DIALOG_HPP
#define CUSTOM_DIALOG_HPP

#include <QDockWidget>

namespace graph_analysis {
namespace gui {

/**
 * \class CustomDialog
 * \file CustomDialog.hpp
 * \brief Specialized QWidget for the properties/commnand panel; implements a dockable widget with keys (and keys combinations) listener
 */
class CustomDialog : public QDockWidget
{
public:
    /// empty constructor
    CustomDialog();
    /// destructor
    ~CustomDialog() {}
protected:
    /// qt keys press callback
    void keyPressEvent(QKeyEvent *event);
};

} // end namespace gui
} // end namespace graph_analysis

#endif /* CUSTOM_DIALOG_HPP */
