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

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget* parent = 0);
    ~MainWindow();

private slots:
    void organizationModelChanged();

    void loadOntology();
    void addFilter();
    void activateNodeFilter(QTableWidgetItem* item);
    void activateEdgeFilter(QTableWidgetItem* item);

    // Allow using QSignalMapper
    void activateNodeFilter(int row);
    void activateEdgeFilter(int row);

    void removeFilter();
    void updateFromModel();

private:
    Ui::Omviz* mUiOmviz;
    owl_om::OrganizationModel::Ptr mpOrganizationModel;

    GraphWidget* mGraphWidget;
    PlanningWidget* mPlanningWidget;
    OrganizationModelWidget* mOrganizationModelWidget;
    PropertiesWidget* mPropertiesWidget;
};

} // end namespace omviz
#endif // OMVIZ_MAINWINDOW_HPP
