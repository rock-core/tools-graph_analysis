#ifndef CUSTOM_DIALOG_HPP
#define CUSTOM_DIALOG_HPP

#include <QDockWidget>

namespace graph_analysis {
namespace gui {

class CustomDialog : public QDockWidget
{
public:
    CustomDialog();
    ~CustomDialog() {}
protected:
    void keyPressEvent(QKeyEvent *event);
};

} // end namespace gui
} // end namespace graph_analysis

#endif /* CUSTOM_DIALOG_HPP */
