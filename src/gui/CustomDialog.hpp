#ifndef CUSTOM_DIALOG_HPP
#define CUSTOM_DIALOG_HPP

#include <QDockWidget>

namespace graph_analysis {
namespace gui {

/**
 * \file CustomDialog.hpp
 * \class CustomDialog
 * \brief Specialized QWidget for the properties/commnand panel
 * \details implements a dockable widget with keypress (and keys combinations) listener
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
