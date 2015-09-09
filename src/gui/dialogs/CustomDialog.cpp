#include "CustomDialog.hpp"

#include <QKeyEvent>

namespace graph_analysis {
namespace gui {
namespace dialogs {

CustomDialog::CustomDialog()
{
    setFeatures(QDockWidget::AllDockWidgetFeatures);
}

void CustomDialog::keyPressEvent(QKeyEvent *event)
{
    // check for a keys combination
    Qt::KeyboardModifiers modifiers = event->modifiers();

    if(modifiers & Qt::ControlModifier)
    {
        switch(event->key())
        {
            case Qt::Key_Q:
                exit(0);
            break;

            case Qt::Key_W:
                close();
            break;
        }
    }
    else if(modifiers & Qt::AltModifier)
    {
        switch(event->key())
        {
            case Qt::Key_F4:
                close();
            break;
        }
    }
    switch(event->key())
    {
        case Qt::Key_Escape:
        case Qt::Key_Backspace:
            close();
        break;

        case Qt::Key_F4:
            close();
        break;
    }
}

} // end namespace dialogs
} // end namespace gui
} // end namespace graph_analysis
