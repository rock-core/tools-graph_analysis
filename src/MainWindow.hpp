#ifndef OMVIZ_MAINWINDOW_HPP
#define OMVIZ_MAINWINDOW_HPP

#include <QtGui/QMainWindow>
#include <QDir>
#include <owl_om/OrganizationModel.hpp>

class QTableWidgetItem;

namespace Ui
{
    class Omviz;
}

namespace omviz
{

class GraphWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
        MainWindow(QWidget* parent = 0);
        ~MainWindow();

private slots:
    void loadOntology();
    void addFilter();
    void activateNodeFilter(QTableWidgetItem* item);
    void removeFilter();

private:
    Ui::Omviz* mUiOmviz;
    owl_om::OrganizationModel mOrganizationModel;

    GraphWidget* mGraphWidget;
};

} // end namespace omviz
#endif // OMVIZ_MAINWINDOW_HPP
