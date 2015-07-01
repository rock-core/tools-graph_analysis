#include "CustomDialog.hpp"


namespace graph_analysis {
namespace gui {

CustomDialog::CustomDialog()
{
    ((QDockWidget *)this)->setFeatures(QDockWidget::AllDockWidgetFeatures);
}

} // end namespace gui
} // end namespace graph_analysis
