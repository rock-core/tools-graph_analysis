#ifndef GRAPH_ANALYSIS_GUI_CUSTOMTABWIDGET_H
#define GRAPH_ANALYSIS_GUI_CUSTOMTABWIDGET_H

#include <QTabWidget>
#include <QMainWindow>

namespace graph_analysis {
namespace gui {

class CustomTabWidget : public QTabWidget
{
public:
    CustomTabWidget(QMainWindow * mainWindow, QWidget *parent = 0);
    ~CustomTabWidget();

protected:
    void currentChanged(int index);

private:
    QMainWindow *mpMainWindow;
};

} // end namespace gui
} // end namespace graph_analysis
#endif // GRAPH_ANALYSIS_GUI_CUSTOMTABWIDGET_H
