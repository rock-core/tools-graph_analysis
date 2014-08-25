#ifndef OMVIZ_MAINWINDOW_HPP
#define OMVIZ_MAINWINDOW_HPP

#include <QtGui/QMainWindow>
#include <QDir>
#include <owl_om/OrganizationModel.hpp>

class QTableWidgetItem;
class QTreeWidget;

namespace Ui
{
    class Omviz;
}

namespace omviz
{

class GraphWidget;
class PlanningWidget;
class OrganizationModelWidget;
class PropertiesWidget;
class FilterWidget;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void organizationModelChanged();
    void organizationModelSelectionChanged(QString parentItem, QString currentItem);

    void loadOntology();
    void updateFilters();
    void updateFromModel();

private:
    Ui::Omviz* mUiOmviz;
    owl_om::OrganizationModel::Ptr mpOrganizationModel;

    GraphWidget* mGraphWidget;
    PlanningWidget* mPlanningWidget;
    OrganizationModelWidget* mOrganizationModelWidget;
    PropertiesWidget* mPropertiesWidget;
    FilterWidget* mFilterWidget;
};

} // end namespace omviz
#endif // OMVIZ_MAINWINDOW_HPP
