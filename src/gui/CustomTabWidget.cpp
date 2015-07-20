#include "CustomTabWidget.hpp"

namespace graph_analysis {
namespace gui {

CustomTabWidget::CustomTabWidget(QMainWindow *mainWindow, QWidget *parent)
    : QTabWidget(parent)
    , mpMainWindow(mainWindow)
{

}

CustomTabWidget::~CustomTabWidget()
{

}

void CustomTabWidget::currentChanged(int index)
{
}

} // end namespace gui
} // end namespace graph_analysis